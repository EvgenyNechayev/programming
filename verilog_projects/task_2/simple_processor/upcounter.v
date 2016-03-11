
module upcounter
	(
	input  wire 		Clear,
	input  wire 		Clock,
	input  wire 		En,
	
	output reg	[1:0] Q
	);

	always @(posedge Clock)
	begin
		if (Clear)
			begin
				Q <= 2'b00;
			end
		else if (En)
			begin
				Q <= Q + 1'b1;
			end
	end
	
endmodule
