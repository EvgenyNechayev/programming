
module regn
	#(parameter N = 16)
	(
		input  wire [N-1:0] R,
		input  wire 		  Rin,
		input  wire 		  Clock,
		input  wire			  Rst,
		
		output reg  [N-1:0] Q
	);

	always @(posedge Clock)
	begin
		if (Rst)
			begin
				Q <= 0;
			end
		else if (Rin)
			begin
				Q <= R;
			end
	end

endmodule
