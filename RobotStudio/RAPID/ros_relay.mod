MODULE ros_relay

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

RECORD RobotData
	num type;
	string status;
ENDRECORD

PERS sensor AnyDevice;
PERS RobotData dataOut := [0, "Hello"];
VAR socketdev server_socket;
VAR socketdev client_socket;
VAR string client_ip;
VAR string receive_string;

PROC relay_main()
	SocketCreate server_socket;
	TCP_init;
	TPWrite "Waiting for client connection";
	SocketAccept server_socket, client_socket, \ClientAddress:=client_ip;
	TPWrite "Client connected.";
	while ( true ) do
		!Receive string from client
		SocketReceive client_socket \Str := receive_string;
		!Reply to client
		SocketSend client_socket \Str := "Hello client with ip-address "+client_ip;
	endwhile
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

PROC TCP_init()
	SocketBind server_socket, "192.168.0.50", 3100;
	SocketListen server_socket;
	TPWrite "Server initialized.";
ENDPROC

PROC handle_packet(Str packetString)
	
ENDPROC

PROC RRI_Open()
	SiConnect AnyDevice;
	! Send and receive data cyclic with 64 ms rate
	SiSetCyclic AnyDevice, dataOut, 64;
ENDPROC

PROC RRI_Close()
	! Close the connection
	SiClose AnyDevice;
ENDPROC


ENDMODULE
