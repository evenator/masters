MODULE MainModule
!This module is where the main program call is held for the two different modes of operation

PROC main ()
        ! LOCAL VARIABLE DECLARATIONS & DEFINITIONS
		VAR num runMode;

		!!Initialize the starting Pose
        Initialize_Pose;
		
		!Assign Variables and open GSI connection
        SiTool := Surg_Tool;
        SiWobj := wobj0;
        RsOpen;


		!Input motion Flag
        RsSensData.PtnData.Item1 := 0;		
		!Busy, Ack Flags
        RsRobData.PtnData.Item19 := 1;  
        RsRobData.PtnData.Item20 := 0;


		!At this point run either program A or Program B.
		TPErase;
        TPReadFK runMode, "Select Operating Mode:", "Trap", "Normal", "", "", "";

		TEST runMode
			CASE 1:
        		TrapMain;
			CASE 2:
        		SurgMain;
		ENDTEST
		
	ENDPROC

ENDMODULE