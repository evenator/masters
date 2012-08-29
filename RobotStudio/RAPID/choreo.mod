MODULE choreo

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

PROC stow()
	VAR robtarget stow_targ := [[-646.44,17.94,299.84],[0.253051,-0.542282,-0.157717,0.785506],[-1,0,1,4],[9E+09,9E+09,9E+09,9E+09,9E+09,9E+09]];
	MoveJ stow_targ, v1000, z30, tool0;
ENDPROC

PROC goto_table1()
	VAR robtarget grab_targ := [[-41.34,1.20,660.90],[0.20385,0.71582,0.209002,0.634322],[-1,0,1,4],[9E+09,9E+09,9E+09,9E+09,9E+09,9E+09]];
	MoveJ grab_targ, v1000, z30, tool0;
ENDPROC

PROC goto_table2()
	VAR robtarget grab_targ := [ [ -40, 0, 695] , [.09232, .73506, .13644, .65769] , [0, 0, 1, 4], [9E9, 9E9, 9E9, 9E9, 9E9, 9E9] ];
	MoveJ grab_targ, v1000, z30, tool0;
ENDPROC

PROC choreo_main()
	VAR num approach_time := 5;
	VAR num grasp_time := 1;
	VAR num travel_time := 20;
	VAR num release_time := 1;
	
	stow;
	WaitTime approach_time;
	goto_table1;
	WaitTime grasp_time;
	stow;
	WaitTime travel_time;
	goto_table2;
	WaitTime release_time;
ENDPROC

ENDMODULE