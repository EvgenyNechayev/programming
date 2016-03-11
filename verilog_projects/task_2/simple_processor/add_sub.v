
module add_sub
	#(parameter N = 16)
	(
		input  wire [N-1:0] A,
		input  wire [N-1:0] B,
		input  wire			  Sub,
		
		output wire	[N-1:0] Q
	);
	
	assign Q = (Sub) ? A - B : A + B;

endmodule
