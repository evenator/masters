MODULE SURG_TRAP
 	var num TrapStateS;  !0. means that the system is ready to get its next command.
						!1. means that the next path is queued up
						!2. means that the next path is being loaded
						!3. means the system recieved the exit command.
	var num TrapCommandS;
	VAR robtarget TempTS;
	VAR jointtarget TempTSJ;
	VAR intnum intno1S; 
	VAR num mTime;


 PROC TrapMain ()
        ! LOCAL VARIABLE DECLARATIONS & DEFINITIONS
	  	VAR num scaleForce := 20;
        VAR num val;
        VAR num maxD;
		VAR num mTimeL;
		VAR orient myorient;
		VAR orient normmyorient;
		VAR robtarget NextT;
		VAR zonedata  zoneRunning;
		
		
		TPErase;
		TPReadFK val, "Select Zone Size:", "1 mm", "2 mm", " 3 mm", "4 mm", "5 mm"; 
		
		TEST val

            CASE 1:
                 zoneRunning := zone01; 
                 !SetDO UserDO, 1;
            CASE 2:
                 zoneRunning := zone02;
			CASE 3:
                 zoneRunning := zone03;
			CASE 4:
                 zoneRunning := zone04;
			CASE 5:
                 zoneRunning := zone05;
        ENDTEST
		
		
		!First step. Start Secondary thread and start RRI.

		mTime := 0.5;

		
        Surg_ACActive := 0;
		

		!Initialize IO vars for the sensor and Robot.
		
        RsSensData.PtnData.Item1 := 0;		
        RsRobData.PtnData.Item7 := 0;
        RsRobData.PtnData.Item19 := 1;
        RsRobData.PtnData.Item20 := 0;
		TrapStateS := 0;
		!Begin Trap Function 10 Hz target speed. (This can be adjusted to be up to 10 Hz but no higher). 
		CONNECT intno1S WITH QueueNextMoveS;
		ITimer .1, intno1S;


		!Move Robot to starting position.
		MoveJ Surg_TopPos, v10\T:= 2, fine, Surg_Tool;


		!The wait function here will be small and is used to wait until the program has a chance to 
		!execute.
        WHILE TrapStateS <> 3 DO
        	! Wait for command from host PC
			WHILE TrapStateS = 0 DO
				WaitTime 0.001;
				IF Surg_StopDI = 1 THEN
			    	GOTO SurgEnd;
				ENDIF
			ENDWHILE

			IF TrapStateS = 3 THEN
				GOTO SurgEnd;
			ENDIF
            
		
			!NacEndReadGSI;
            ! ACK the server's command
            ! Once the NacReadForceGSI command is called, we won't be
            ! able to update RsRobData, so ack before activating it           

            !NacReadForceGSI;




            ! Force-controlled Cartesian linear move
            ! move,            //! 1 Issue move command (move = 2, nomove = 0, killmove = -1)
            ! target_->method, //! 2 PositionControl = 0, ForceControl = 1, Rotation = 2, Gripper = 3, NoMethod = 4
            ! target_->x,      //! 3 Absolute X coordinate || Gripper open/close
            ! target_->y,      //! 4 Absolute Y coordinate
            ! target_->z,      //! 5 Absolute Z coordinate
            ! target_->rz,     //! 6 Absolute X axis rotation
            ! target_->ry,     //! 7 Absolute Y axis rotation
            ! target_->rz,     //! 8 Absolute Z axis rotation

		TPWrite "Robot Command Interp";		
			!All movement
            TEST TrapCommandS
			
			CASE 1:
				! *****************************************************
                ! *          Position Control Cartesian Move          *
                ! *      Linear move with rotation capabilities       *
                ! *****************************************************
		    	TrapStateS :=2;
				NextT := TempTS;
				mTimeL := mTime;
				TrapCommandS := -1;
				TrapStateS := 0;
				ConfL \Off;
				!moveL NextT, v100, \T:=mTime, z5, Surg_Tool;
				SetDO UserDO2, 0;
				MoveLDO NextT, v100, \T:=mTime, zoneRunning, Surg_Tool, UserDO2, 1;
            
			CASE 10:
                ! *****************************************************
                ! *          Position Control Cartesian Move          *
				! *					with Quaternions                  *	
                ! *      Linear move with rotation capabilities       *
                ! *****************************************************
                TPWrite "QuatMove";	
				IF Surg_ACActive = 1 THEN
                    ! Disable AC
                    FCDeact;
                    Surg_ACActive := 0;
                ENDIF
                TrapStateS :=2;
				NextT := TempTS;
				mTimeL := mTime;
				TrapCommandS := -1;
				TrapStateS := 0;
				ConfL \Off;
				!moveL NextT, v100, \T:=mTime, z5, Surg_Tool;
				SetDO UserDO2, 0;
				MoveLDO NextT, v100\T:=mTime, zoneRunning, Surg_Tool, UserDO2, 1;
			
			CASE 11:
				TrapStateS :=2;
				NextT := TempTS;
				mTimeL := mTime;
				TrapCommandS := -1;
				TrapStateS := 0;
				ConfL \Off;
				!moveL NextT, v100, \T:=mTime, z5, Surg_Tool;
				SetDO UserDO2, 0;
				MoveJDO NextT, v100\T:=mTime, zoneRunning, Surg_Tool, UserDO2, 1;
			
			
			
			!Multiple move types will use this final command.
			DEFAULT:
				IF Surg_ACActive = 1 THEN
                    ! Disable AC
                    FCDeact;
                    Surg_ACActive := 0;
                ENDIF
                TrapStateS :=2;
				NextT := TempTS;
				mTimeL := mTime;
				TrapCommandS := -1;
				TrapStateS := 0;
				ConfL \Off;
				!moveL NextT, v100, \T:=mTime, z5, Surg_Tool;
				SetDO UserDO2, 0;
				MoveLDO NextT, v100\T:=mTime, zoneRunning, Surg_Tool, UserDO2, 1;
				
				
				

            ENDTEST
		

			!SurgDone:
            !NacEndReadGSI;
           

        ENDWHILE
SurgEnd:
        !SetDO UserDO, 0;
        TPWrite "Disconnecting from server";
        ! Turn off force control if it's on
        IF Surg_ACActive = 1 THEN
            FCDeact;
            Surg_ACActive := 0;
        ENDIF

        ! Close GSI connection
        RsClose;
		!Release Intterrupt
		IDelete intno1s;
		
		
    ENDPROC

	!Trap Function

TRAP QueueNextMoveS

	
	
	 TEST TrapStateS
        
		CASE 0:
			IF RsSensData.PtnData.Item1 > 0 THEN
				tempData := RsSensData.PtnData;
				IF RsSensData.PtnData.Item1 = 3 THEN
					TrapStateS := 3;
					tempData.Item2 := -1;	
				ENDIF
				TPWrite "Got SomeData";
				
				RsRobData.PtnData.Item19 := 0; 
				RsRobData.PtnData.Item20 := 1;
				RsSensData.PtnData.Item1 :=0;
				TEST tempData.Item2
				CASE 0:
                	! *****************************************************
                	! *          Position Control Cartesian Move          *
                	! *      Linear move with rotation capabilities       *
                	! *****************************************************
                	!TPWrite "PC Cartesian Move";
                	
					TempTS := PC_CartMove(tempData);
					IF mTime < .2 mTime  := .2;
					TrapStateS := 1;
					TrapCommandS := 1;
				CASE 3:
                	! *****************************************************
                	! *                Gripper Actuation                  *
                	! *    No movement.  Just open or close the gripper   *
                	! *****************************************************
                	TPWrite "Gripper Actuation";
                	IF tempData.Item3 > 0.5 THEN
                    	SetDO UserDO1, 0;
                	ELSE
                    	SetDO UserDO1, 1;
                	ENDIF
					
					
				
				CASE 10:
		    		! *****************************************************
            		! *          Position Control Cartesian Move          *
					! *					with Quaternions                  *	
            		! *      Linear move with rotation capabilities       *
            		! *****************************************************
					!TPWrite "Prepping a QuatMove";
					
					TempTS := PC_QuatMove(tempData);
					mTime := tempdata.Item13;
					IF mTime < .2 mTime  := .2;
					TrapStateS := 1;
					TrapCommandS := 10;
					
					
				CASE 11:
                ! *****************************************************
                ! *          Joint Control Move                       *
				! *					                                  *	
                ! *                                                   *
                ! *****************************************************
					TempTS := PC_JointMove(tempData);
					mTime := tempdata.Item13;
					IF mTime < .2 mTime  := .2;
					TrapStateS := 1;
					TrapCommandS := 11;
						
					DEFAULT:
					!If the command is not reconized it will be thrown away.
					
				ENDTEST
				tempData.Item2:= -1;
		
			ELSE
				!Output proper status flass
				RsRobData.PtnData.Item19 := 1; 
				RsRobData.PtnData.Item20 := 0;
				!Output the forces
				myForceVector := FCGetForce(\Wobj:=wobj0 \ContactForce);
				RsRobData.PtnData.Item13 := myForceVector.xforce;
				RsRobData.PtnData.Item14 := myForceVector.yforce;
				RsRobData.PtnData.Item15 := myForceVector.zforce;
				!! Torque need to be added here we need to find available Items
				!---> Torque Output
				
				!!Position Data Output
				p0 := CRobT(\Tool:=Surg_Tool \Wobj:=wobj0);
				RsRobData.PtnData.Item9 := p0.rot.q1;
				RsRobData.PtnData.Item10 := p0.rot.q2;
				RsRobData.PtnData.Item11 := p0.rot.q3;
				RsRobData.PtnData.Item12 := p0.rot.q4;
			ENDIF
		CASE 1:
		
		
		
		!CASE 2: Do Nothing for Case 2
		DEFAULT :
		
		ENDTEST
	
	
	ENDTRAP


ENDMODULE