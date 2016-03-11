
module demultiplexer (
	input wire [7:0] data_i,
	input wire [1:0] sel_i,
	input wire enable_i,
	
	output reg [7:0] data_a_o,
	output reg [7:0] data_b_o,
	output reg [7:0] data_c_o,
	output reg [7:0] data_d_o
);

	always @*
	begin
		if (enable_i)
		begin
			case (sel_i)
			2'b00: data_a_o = data_i;
			2'b01: data_b_o = data_i;
			2'b10: data_c_o = data_i;
			2'b11: data_d_o = data_i;
			endcase
		end
	end
	
endmodule
