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

RECORD jointTrajectoryPt
	jointtarget joint_pos;
	num velocity;
	bool stop;
ENDRECORD

LOCAL VAR socketdev server_socket;
LOCAL VAR socketdev client_socket;
LOCAL VAR num server_port := 11000;
LOCAL VAR rawbytes buffer;
VAR jointTrajectoryPt nextPoint;
LOCAL VAR jointTrajectoryPt trajectoryHead;
LOCAL VAR jointTrajectoryPt trajectoryTail;
PROC joint_trajectory_server_main()
	SocketCreate server_socket;
	TCP_init;
	TPWrite "Waiting for client connection";
	SocketAccept server_socket, client_socket, \ClientAddress:=client_ip;
	TPWrite "Client connected.";
	WHILE ( true ) DO
		!Recieve Joint Trajectory Pt Message
		SocketReceive client_socket \RawData:=buffer \Time:=WAIT_MAX;
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

ENDMODULE