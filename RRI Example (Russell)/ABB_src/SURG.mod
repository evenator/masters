MODULE SURG
    VAR fcboxvol srchbox;
    VAR robtarget p0;
	VAR jointtarget JntTarget; 
    VAR num counter;
    VAR num xmin;
    VAR num xmax;
    VAR num ymin;
    VAR num ymax;
    VAR num zmin;
    VAR num zmax;
	VAR fcforcevector myForceVector;
    VAR patterndata tempData;
    VAR patterndata oldData;

    

    PROC SurgTestConfig ()
        VAR num myerr;
        VAR num key_num;

        TPReadFK key_num,"Select task:", "Poke", "Grip", "", "", ""; 

        ! Deactivate force control if already active
        IF Surg_ACActive = 1 THEN
            ! Disable AC
            FCDeact;
            Surg_ACActive := 0;
        ENDIF

        TEST key_num
            CASE 1:
                 Surg_Load := Surg_Poke_Load;
                 Surg_Tool := Surg_Poke_Tool;
            CASE 2:
                 Surg_Load := Surg_Grip_Load;
                 Surg_Tool := Surg_Grip_Tool;
        ENDTEST

        FCCalib Surg_Load;
        FCAct Surg_Tool;

        TPReadFK key_num, "Finished?", "", "", "Okay", "", "";
        FCDeact;
    ENDPROC


    PROC SurgPaperTest ()
        VAR num myerr;
        VAR num key_num;

        ! Deactivate force control if already active
        IF Surg_ACActive = 1 THEN
            ! Disable AC
            FCDeact;
            Surg_ACActive := 0;
        ENDIF

        Surg_Load := Surg_Poke_Load;
        Surg_Tool := Surg_Poke_Tool;

        ! Calibrate the force control for the tool load
        FCCalib Surg_Load;
        ! Activate force control for the tool
        FCAct Surg_Tool;

        ! Move down until we touch the surface
        ! Push down with 5 N of force
        FCRefForce \Fz:=5.0;
        ! 
        FCCondForce \ZMin:=-1.0 \ZMax:=3.0, 10.0;
        FCRefStart;
        FCCondWaitWhile \ZeroRefAtEnd;
        FCRefStop;

        !Push the paper forward
        srchbox := [xmin, xmax, ymin, ymax, zmin, zmax];
        FCRefForce \Fx:=10.0 \Fz:=3.0;
        FCCondPos \Box:=srchbox, 10.0;
        FCRefStart;
        FCCondWaitWhile \ZeroRefAtEnd;
        FCRefStop;

        ! Turn off force control
        FCDeact;
    ENDPROC


    PROC SurgHybridTest ()
        VAR num myerr;
        VAR num key_num;
        VAR num rx;
        VAR num ry;
        VAR num rz;

        TPReadFK key_num,"Select task:", "Poke", "Grip", "", "", ""; 

        ! Deactivate force control if already active
        IF Surg_ACActive = 1 THEN
            ! Disable AC
            FCDeact;
            Surg_ACActive := 0;
        ENDIF

        TEST key_num
            CASE 1:
                 Surg_Load := Surg_Poke_Load;
                 Surg_Tool := Surg_Poke_Tool;
            CASE 2:
                 Surg_Load := Surg_Grip_Load;
                 Surg_Tool := Surg_Grip_Tool;
        ENDTEST

        FCCalib Surg_Load;
        FCAct Surg_Tool;

        p0 := CRobT(\Tool:=Surg_Tool \Wobj:=wobj0);
        rx := EulerZYX(\X, p0.rot);
        ry := EulerZYX(\Y, p0.rot);
        rz := EulerZYX(\Z, p0.rot) + 90.0;
        p0.rot := OrientZYX (rz, ry, rx);
        MoveL p0, v100, fine, Surg_Tool, \WObj:=wobj0;
 
        TPReadFK key_num, "Finished?", "", "", "Okay", "", "";
        FCDeact;
    ENDPROC


    PROC ComTest ()
        VAR num key1;

        TPReadFK key1,"Select task:", "Poke", "Grip", "", "", ""; 

        ! Deactivate force control if already active
        IF Surg_ACActive = 1 THEN
            ! Disable AC
            FCDeact;
            Surg_ACActive := 0;
        ENDIF

        TEST key1
            CASE 1:
                 Surg_Load := Surg_Poke_Load;
                 Surg_Tool := Surg_Poke_Tool;
            CASE 2:
                 Surg_Load := Surg_Grip_Load;
                 Surg_Tool := Surg_Grip_Tool;
        ENDTEST

        FCCalib Surg_Load;
        FCAct Surg_Tool;

        ! Open GSI connection
        RsOpen;

        ! Start force communication
        !NacReadForceGSI;
        !TPReadFK key1, "Done? ", "", "", "Okay", "", "";
        !NacEndReadGSI;
        FCDeact;

        ! Close GSI connection
        RsClose;
    ENDPROC



    PROC SurgLoadCalibrate()
        VAR num myerr;
        VAR num key_num;

        TPReadFK key_num, "Select task:", "Poke", "Grip", "Tool", "", ""; 

        ! Deactivate force control if already active
        IF Surg_ACActive = 1 THEN
            ! Disable AC
            FCDeact;
            Surg_ACActive := 0;
        ENDIF

        TEST key_num
            CASE 1:
                 Surg_Poke_Load := FCLoadId (\MaxMoveAx5:=60 \MaxMoveAx6:=120
                                   \ReadingsPerPoint:=1 \PointsPerAxis:=7 \loadidErr:=myerr);
                 Surg_Load := Surg_Poke_Load;
            CASE 2:
                 Surg_Grip_Load := FCLoadId (\MaxMoveAx5:=60 \MaxMoveAx6:=120 \ReadingsPerPoint:=7
                                   \PointsPerAxis:=7 \loadidErr:=myerr);
                 Surg_Load := Surg_Grip_Load;
            CASE 3:
            	Surg0_Load := FCLoadId (\MaxMoveAx5:=60 \MaxMoveAx6:=120 \ReadingsPerPoint:=7
                                   \PointsPerAxis:=7 \loadidErr:=myerr);
                 Surg_Load := Surg0_Load;
                 
        ENDTEST   

        TPWrite "mass = " \Num := Surg_Load.mass;
        TPWrite "x = " \Num := Surg_Load.cog.x;
        TPWrite "y = " \Num := Surg_Load.cog.y;
        TPWrite "z = " \Num := Surg_Load.cog.z;
        TPWrite "err = " \Num := myerr;
    ENDPROC


    PROC SurgMain ()
        ! LOCAL VARIABLE DECLARATIONS & DEFINITIONS
	  	VAR num scaleForce := 20;
        VAR num val;
        VAR num xf;
        VAR num yf;
        VAR num zf;
        VAR num maxD;
        VAR num xD;
        VAR num yD;
        VAR num zD;
		VAR num q1;
		VAR num q2;
		VAR num q3;
		VAR num q4;
		VAR orient myorient;
		VAR orient normmyorient;
		
        Surg_ACActive := 0;

		!Set output variables.(Current state = not busy)
		
		!Input motion Flag
        RsSensData.PtnData.Item1 := 0;		
		!Busy, Ack Flags
        RsRobData.PtnData.Item19 := 1;  
        RsRobData.PtnData.Item20 := 0;

		
		!Move_Robot To Starting Pose
		MoveJ Surg_TopPos, v30, fine, Surg_Tool;

	
		! Wait for command from host PC
		! wait for command for WaitTime .001 seconds
        WHILE RsSensData.PtnData.Item1 >= 0 DO

            WaitTime 0.001;
            RsRobData.PtnData.Item1 := 0;        

		
		!!This while loop outputs the force sensor data.
		! In reall time execution.
		WHILE RsSensData.PtnData.Item1 = 0 DO
		 ! Signal to server that commanded move is finished
            RsRobData.PtnData.Item19 := 1;
            RsRobData.PtnData.Item20 := 0;
		 
		 
		
		!myForceVector := FCGetForce(\Tool:=Surg_Tool);
		!Output the forces
		myForceVector := FCGetForce(\Wobj:=wobj0 \ContactForce);
		RsRobData.PtnData.Item13 := myForceVector.xforce;
		RsRobData.PtnData.Item14 := myForceVector.yforce;
		RsRobData.PtnData.Item15 := myForceVector.zforce;
		!! Torque need to be added here we need to find available Items
		
		!Output the Quaternion position.
        p0 := CRobT(\Tool:=Surg_Tool \Wobj:=wobj0);
		RsRobData.PtnData.Item9 := p0.rot.q1;
        RsRobData.PtnData.Item10 := p0.rot.q2;
        RsRobData.PtnData.Item11 := p0.rot.q3;
        RsRobData.PtnData.Item12 := p0.rot.q4;
		
		
		!Local Quit command
		IF Surg_StopDI = 1 THEN
		    GOTO SurgEnd;
		ENDIF	
		
		!Loop time
		WaitTime 0.001;
		
		
		ENDWHILE !WHILE RsSensData.PtnData.Item1 = 0 DO
		
			!Remote Quit Command.
        	IF RsSensData.PtnData.Item1 = 3 THEN
                !ErrLog 4800, errstr1, errstr1, ERRSTR_UNUSED, ERRSTR_UNUSED, ERRSTR_UNUSED;
                GOTO SurgEnd;
            ENDIF
			
			
            tempData := RsSensData.PtnData;
		
		    ! ACK the server's command
            ! Once the NacReadForceGSI command is called, we won't be
            ! able to update RsRobData, so ack before activating it           
			RsSensData.PtnData.Item1 := 0;
			
			!Update Robot Status to moving
			RsRobData.PtnData.Item19 := 0;
            RsRobData.PtnData.Item20 := 1;
            WaitTime 0.001;
            

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

        TEST tempData.Item2
            CASE 0:
                ! *****************************************************
                ! *          Position Control Cartesian Move          *
                ! *      Linear move with rotation capabilities       *
                ! *****************************************************
			
				p0 := PC_CartMove(tempData);
				MoveL p0, vCal10, fine, Surg_Tool\WObj:=wobj0;
			
			
            CASE 1:
                ! *****************************************************
                ! *            Force Control Linear Move              *
                ! *  Accommodation control lacks rotation capability  *
                ! *****************************************************
                IF Surg_ACActive = 0 THEN
                    ! Enable Accomodation Control
                    FCCalib Surg_Load;
                    FCAct Surg_Tool;
                    Surg_ACActive := 1;
                ENDIF

                TPWrite "FC Linear Move";
                p0 := CRobT(\Tool := Surg_Tool \Wobj := wobj0);

                ! Find the maximum offset value
                IF Abs(tempData.Item3) > Abs(tempData.Item4) THEN
                    maxD := Abs(tempData.Item3);
                ELSE
                    maxD := Abs(tempData.Item4);
                ENDIF                 

                IF Abs(tempData.Item5) > maxD THEN
                    maxD := Abs(tempData.Item5);
                ENDIF

                xD := (scaleForce / maxD) * tempData.Item3;
                yD := (scaleForce / maxD) * tempData.Item4;
                zD := (scaleForce / maxD) * tempData.Item5;

                ! X offset
                IF Abs(tempData.Item3) <= 0.001 THEN
                    xmin := -9e9;
                    xmax := 9e9;
                    xf := 0;
                ELSE
                    IF tempData.Item3 > 0 THEN
                        xmin := -9e9;
                        xmax := p0.trans.x + tempData.Item3;
                    ELSE
                        xmin := p0.trans.x + tempData.Item3;
                        xmax := 9e9;
                    ENDIF
                    xf := xD;
                ENDIF

                ! Y offset
                IF Abs(tempData.Item4) <= 0.001 THEN
                    ymin := -9e9;
                    ymax := 9e9;
                    yf := 0;
                ELSE
                    IF tempData.Item4 > 0 THEN
                        ymin := -9e9;
                        ymax := p0.trans.y + tempData.Item4;
                    ELSE
                        ymin := p0.trans.y + tempData.Item4;
                        ymax := 9e9;
                    ENDIF
                    yf := yD;
                ENDIF

                ! Z offset
                IF Abs(tempData.Item5) <= 0.001 THEN
                    zmin := -9e9;
                    zmax := 9e9;
                    zf := 0;
                ELSE
                    IF tempData.Item5 > 0 THEN
                        zmin := -9e9;
                        zmax := p0.trans.z + tempData.Item5;
                    ELSE
                        zmin := p0.trans.z + tempData.Item5;
                        zmax := 9e9;
                    ENDIF
                    zf := zD;
                ENDIF

                srchbox := [xmin, xmax, ymin, ymax, zmin, zmax];
                FCRefForce \Fx := xf \Fy := yf \Fz := zf;

                ! If we want to specify a move until touch motion
                ! FCCondForce \XMin:=tempData.Item10 \XMax:=tempData.Item11
                !             \YMin:=tempData.Item12 \YMax:=tempData.Item13
                !             \ZMin:=tempData.Item14 \ZMax:=tempData.Item15, tempData.Item18;
                FCCondPos \Box := srchbox, 30.0;
                FCRefStart;
                FCCondWaitWhile \ZeroRefAtEnd;
                FCRefStop;
            CASE 2:
                ! *****************************************************
                ! *            Position Control Rotation              *
                ! *        No translation, just pure rotation         *
                ! *****************************************************


                p0 := PC_CartRot(tempData);
                MoveL p0, v100, fine, Surg_Tool, \WObj:=wobj0;
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
				WaitTime 0.05;

			Case 5:
 		    	! *****************************************************
            	! *                Set Compliance Mode                *
            	! * No movement.  Just turn force Control on and off  *
            	! *****************************************************
            	TPWrite "Setting Compliance";
		    	IF tempData.Item3 = 0 THEN
		        	!Disable AC.
		    	  	IF Surg_ACActive = 1 THEN
                    	! Disable AC
                    	FCDeact;
                    	Surg_ACActive := 0;
			  	ENDIF
		    	ELSE
		    		!enable AC
			  		IF Surg_ACActive = 0 THEN
                    	! Enable AC
                    	FCCalib Surg_Load;
                        FCAct Surg_Tool;
                    	Surg_ACActive := 1;
			  		ENDIF	  
                ENDIF
            CASE 7:
                ! *****************************************************
                ! *                  Guided Motion                    *
                ! *    Multiple Parts first, go down until the table  *
		    	! *    is hit, then await instructions.               *
                ! *****************************************************
                IF Surg_ACActive = 1 THEN
                    ! Disable AC
                    FCDeact;
                    Surg_ACActive := 0;
                ENDIF
		    ! Now that that is done, lower until touching the table. 
		    FCCalib Surg_Load;
		    
		    FCCondForce \Zmin := -2 \Zmax := 2, 80;
		    FCRefLine FC_LIN_Z, -5, 90;
		    
		    FCAct Surg_Tool;
		    FCRefStart;
		    FCCondWaitWhile;
		    FCRefStop;
		    FCDeact;
		    TPWrite "Table Contact";
            CASE 8:
                ! *****************************************************
                ! *               Go to start position                *
                ! *             Thats all there is to it              *
		    ! *                                                   *
                ! *****************************************************
		    IF Surg_ACActive = 1 THEN
                    ! Disable AC
                    FCDeact;
                    Surg_ACActive := 0;
                ENDIF	
		    MoveL Surg_StartPos, vCal10, fine, Surg_Tool\WObj:=wobj0;

            CASE 9:
                ! *****************************************************
                ! *               Begin linear seek                   *
                ! *            The goal is to move in the -Y          *
		    ! * the Robot will rely on ext/EMG stop conditions    *
                ! *****************************************************
		    IF Surg_ACActive = 1 THEN
                    ! Disable AC
                    FCDeact;
                    Surg_ACActive := 0;
                ENDIF
		    !to start with begin a search in the -y direction based. 
		    ymin := RsRobData.PtnData.Item4;
		    
		    FCCalib Surg_Load;
		    FCRefLine FC_LIN_Y, -2, ymin;
		    
		
		    FCAct Surg_Tool;
		    FCRefStart;
		    WaitTime ymin/2;
		    FCRefStop;
		    FCDeact;
		    TPWrite "ready to grip_Gel";
		    	
			CASE 10:
                ! *****************************************************
                ! *          Position Control Cartesian Move          *
				! *					with Quaternions                  *	
                ! *      Linear move with rotation capabilities       *
                ! *****************************************************
                IF Surg_ACActive = 1 THEN
                    ! Disable AC
                    FCDeact;
                    Surg_ACActive := 0;
                ENDIF
                
				p0  := PC_QuatMove(tempData);

                IF tempdata.item13 > 0 THEN
                	MoveL p0, v5000\T:=tempdata.item13, fine, Surg_Tool\WObj:=wobj0;
                ELSE
                	MoveL p0, vCal10, fine, Surg_Tool\WObj:=wobj0;
                ENDIF
			CASE 11:
                ! *****************************************************
                ! *          Joint Control   Move                     *
				! *					                                  *	
                ! *                                                   *
                ! *****************************************************
				!TPWrite "Joint Motion";
				p0 := PC_JointMove(tempData);
				!JntTarget :=CalcJointT(p0,Surg_Tool);

				IF tempdata.item13 > 0 THEN
                	MoveJ p0, v5000\T:=tempdata.item13, fine, Surg_Tool\WObj:=wobj0;
                	TPWrite "Time motion";
                ELSE
                	MoveJ  p0, v50, fine, Surg_Tool\WObj:=wobj0;
                ENDIF
				!TPWrite "Finished Joint Motion";
				
				
				

            ENDTEST
			tempData.Item2 := -1;

SurgDone:
            !NacEndReadGSI;
            ! Signal to server that commanded move is finished
            RsRobData.PtnData.Item19 := 1;
            RsRobData.PtnData.Item20 := 0;
            WaitTime 0.001;

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
    ENDPROC
ENDMODULE