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
		WaitTime .050;
      !Send Mode to Client
      send_mode;
      !Send Status to Client
      
      !Send EStop State to Client
      
		!SocketSend client_socket \Str := "Hello client with ip-address "+client_ip;
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

PROC send_mode()
   VAR int mode;
   VAR rawbytes message;
   
   TEST OpMode()
      CASE OP_AUTO:
         mode = 0; !auto
      CASE OP_MAN_PROG:
         mode = 1; !manual
      CASE OP_MAN_TEST:
         mode = 1; !manual
      DEFAULT:
         mode = 8; !undefined
   ENDTEST
   
   !Pack data
   PackRawBytes 0, message, (RawBytesLen(message)+1) \IntX := USINT; !message alert level OK
   PackRawBytes 0, message, (RawBytesLen(message)+1) \IntX := USINT; !message type mode
   PackRawBytes mode, message, (RawBytesLen(message)+1) \IntX := DINT; !mode
   PackRawBytes 0, message, (RawBytesLen(message)+1) \IntX := DINT; !Data length 0
   !Send data
   SocketSend client_socket \RawData := message;
ENDPROC

PROC send_estop()
   VAR int estop;
   VAR rawbytes message;
   
   TEST OpMode()
      CASE OP_AUTO:
         mode = 0; !auto
      CASE OP_MAN_PROG:
         mode = 1; !manual
      CASE OP_MAN_TEST:
         mode = 1; !manual
      DEFAULT:
         mode = 8; !undefined
   ENDTEST
   
   !Pack data
   PackRawBytes 0, message, (RawBytesLen(message)+1) \IntX := USINT; !message alert level OK
   PackRawBytes 0, message, (RawBytesLen(message)+1) \IntX := USINT; !message type mode
   PackRawBytes mode, message, (RawBytesLen(message)+1) \IntX := DINT; !mode
   PackRawBytes 0, message, (RawBytesLen(message)+1) \IntX := DINT; !Data length 0
   !Send data
   SocketSend client_socket \RawData := message;
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
