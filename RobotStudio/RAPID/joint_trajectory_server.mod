MODULE joint_trajectory_server

!Copyright (c) 2012, Edward Venator, Case Western Reserve University
!All rights reserved.
!
!Redistribution and use in source and binary forms, with or without modification,
!are permitted provided that the following conditions are met:
!
!	Redistributions of source code must retain the above copyright notice, this
!		list of conditions and the following disclaimer.
!	Redistributions in binary form must reproduce the above copyright notice, this
!		list of conditions and the following disclaimer in the documentation
!		and/or other materials provided with the distribution.
!	Neither the name of the Case Western Reserve University nor the names of its contributors
!		may be used to endorse or promote products derived from this software without
!		specific prior written permission.
!
!THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
!EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
!OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
!SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
!INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
!TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
!BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
!CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
!WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

LOCAL VAR socketdev server_socket;
LOCAL VAR socketdev client_socket;
LOCAL VAR num server_port := 11000;
LOCAL VAR rawbytes buffer;

LOCAL VAR num tmp_max_sequence;
LOCAL VAR jointTrajectoryPt{100} tmp_trajectory;

PROC joint_trajectory_server_main()
	SocketCreate server_socket;
	TCP_init;
	TPWrite "Waiting for client connection";
	SocketAccept server_socket, client_socket, \ClientAddress:=client_ip;
	TPWrite "Client connected.";
	WHILE ( true ) DO
		!Recieve Joint Trajectory Pt Message
		SocketReceive client_socket \RawData:=buffer \Time:=WAIT_MAX;
		trajectory_pt_callback;
	ENDWHILE
	SocketClose server_socket;
	SocketClose client_socket;
	ERROR
		IF ERRNO=ERR_SOCK_TIMEOUT THEN
			RETRY;
		ELSEIF ERRNO=ERR_SOCK_CLOSED THEN
			TCP_init;
			RETRY;
		ELSE
			! No error recovery handling
		ENDIF
ENDPROC

LOCAL PROC TCP_init()
	SocketBind server_socket, server_ip, server_port;
	SocketListen server_socket;
	TPWrite "Server initialized.";
ENDPROC

LOCAL PROC trajectory_pt_callback()
	VAR num index :=0;
	VAR num packet_length;
	VAR num type;
	VAR num reply_code;
	
	VAR num sequence;
	VAR num joint_tmp;
	VAR JointTrajectoryPt point;
	VAR jointtarget current_pos;
	
	UnpackRawBytes buffer, index, packet_length, \IntX:=UDINT;
	index := index + 4;
	UnpackRawBytes buffer, index, type, \IntX:=UDINT;
	index := index + 4;
	index := index + 4; !skip comm type because we don't care
	UnpackRawBytes buffer, index, reply_code, \IntX:=UDINT;
	index := index + 4;
	UnpackRawBytes buffer, index, sequence, \IntX:=DINT;
	index := index + 4;
	
	FOR jointnum from 1 to 10 DO
		IF jointnum <= 6 THEN !The robot has only 6 joints, but the message requires 10
			UnpackRawBytes buffer, index, joint_tmp, \Float4; !Get the joint angle in radians
			SetDataValue "point.joint_pos.rax_"+ValToStr(jointnum), joint_tmp; !Stick the joint angle in our data structure
		ENDIF
		index := index + 4;
	ENDFOR
	UnpackRawBytes buffer, index, point.velocity, \Float4;
	
	TEST sequence
		CASE -1: !Start of download
			point.stop := false; !Don't stop on this point
			sequence := 0; !This is the first point in the sequence
			tmp_max_sequence := sequence; !Increment the max sequence number
			tmp_trajectory{sequence + 1} := point; !Add this point to the trajectory
		CASE -2: !Start of stream
			point.stop := false; !Don't stop on this point
			sequence := 0; !This is the first point in the sequence
			tmp_max_sequence := sequence; !Increment the max sequence number
			tmp_trajectory{sequence + 1} := point; !Add this point to the trajectory
		CASE -3: !End of stream
			point.stop := true; !Stop on this point
			sequence := tmp_max_sequence + 1; !Set sequence number to 1 higher than max
			tmp_trajectory{sequence + 1} := point; !Add this point to the trajectory
			current_trajectory := tmp_trajectory; !Send trajectory to motion process
			max_sequence := tmp_max_sequence + 1; !Send max sequence to motion process
			trajectory_ptr := 0; !Reset trajectory pointer
			tmp_max_sequence := -1; !Reset max sequence
		CASE -4: !Stop command
			!Replace the current trajectory with a trajectory to stop at the current position
			current_pos := CJointT(); !Get the current position
			point.joint_pos := current_post.robax; !Go to the current position
			point.velocity := 0; !Velocity should be 0
			point.stop := true; !Stop on the current position
			current_trajectory{1} := point; !Send trajectory of one point to the motion process
			max_sequence := 0; !Reset max sequence
			trajectory_ptr := 0; !Reset trajectory pointer
			tmp_max_sequence := -1;  !Reset max sequence
		DEFAULT:
			point.stop := false;
			tmp_max_sequence := sequence; !Increment the max sequence number
			tmp_trajectory{sequence + 1} := point; !Add this point to the trajectory
	ENDTEST
	
	IF reply_code > 0 THEN
		SocketSend client_socket \Str := "Message received";
	ENDIF
ENDPROC
	
ENDMODULE