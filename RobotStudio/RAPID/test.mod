MODULE testmod

PROC main()
	CONST robtarget p10 := [ [600, -100, 800], [1, 0, 0, 0], [0, 0, 0,0], [ 9E9, 9E9, 9E9, 9E9, 9E9, 9E9] ];
	MoveL p10, v1000, fine, tool0;
ENDPROC

ENDMODULE