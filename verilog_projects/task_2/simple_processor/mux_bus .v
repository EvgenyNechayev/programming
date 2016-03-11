
module mux_bus
	#(parameter N = 16)
	(
		input wire 			 DINout,
		input wire 		  	 Gout,
		input wire [7:0]	 Rout,
		input wire [N-1:0] R0,
		input wire [N-1:0] R1,
		input wire [N-1:0] R2,
		input wire [N-1:0] R3,
		input wire [N-1:0] R4,
		input wire [N-1:0] R5,
		input wire [N-1:0] R6,
		input wire [N-1:0] R7,
		input wire [N-1:0] G,
		input wire [N-1:0] DIN,
		
		output reg [N-1:0] BusOut
	);
	
	always @*
	begin
		BusOut = 0;
		case ({DINout, Gout, Rout})
		10'b1000000000: BusOut = DIN;
		10'b0100000000: BusOut = G;
		10'b0010000000: BusOut = R7;
		10'b0001000000: BusOut = R6;
		10'b0000100000: BusOut = R5;
		10'b0000010000: BusOut = R4;
		10'b0000001000: BusOut = R3;
		10'b0000000100: BusOut = R2;
		10'b0000000010: BusOut = R1;
		10'b0000000001: BusOut = R0;
		endcase
	end
	
endmodule
