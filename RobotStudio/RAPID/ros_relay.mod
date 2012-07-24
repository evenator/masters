MODULE ros_relay

! A RAPID module for sending and receiving data to/from ROS
! Designed to be run on the IRC5 Compact with a IRB-120 arm
! Author: Edward Venator (esv@case.edu)
! License: Do not distribute/use/reproduce without permission.
! Case Western Reserve University and the author do not provide any guarantee
! that this code will work as advertised or expected. This code is provided
! as-is, and the user accepts all risk and responsibility for any injury or
! property damage that may result from the use of this code.

RECORD RobotData
	num type;
	string status;
ENDRECORD

PERS sensor ros := [1,4,0];
PERS RobotData dataOut := [0, "Hello"];

PROC main()
	RRI_Open;
ENDPROC

PROC RRI_Open()
	SiConnect ros;
	! Send and receive data cyclic with 64 ms rate
	SiSetCyclic ros, dataOut, 64;
ENDPROC

PROC RRI_Close()
	! Close the connection
	SiClose ros;
ENDPROC


ENDMODULE
