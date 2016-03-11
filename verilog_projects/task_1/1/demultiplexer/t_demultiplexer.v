
module test_demultiplexer;

	reg [7:0] data_i;
	reg [1:0] sel_i;
	reg enable_i;
	
	wire [7:0] data_a_o;
	wire [7:0] data_b_o;
	wire [7:0] data_c_o;
	wire [7:0] data_d_o;
	
	demultiplexer demultiplexer_instance (
	.data_i(data_i), .sel_i(sel_i), .enable_i(enable_i),
	.data_a_o(data_a_o), .data_b_o(data_b_o),
	.data_c_o(data_c_o), .data_d_o(data_d_o)
	);
	
	initial
	begin
		data_i = 8'h00;
		sel_i = 2'b00;
		enable_i = 1'b0;
		
		#10
		begin
			data_i = 8'h11;
			enable_i = 1'b1;
		end
		#50	
		begin
			data_i = 8'h22;
			sel_i = 2'b01;
		end
		#50	
		begin
			data_i = 8'h33;
			sel_i = 2'b10;
		end
		#50	
		begin
			data_i = 8'h44;
			sel_i = 2'b11;
		end
		#10
		begin
			data_i = 8'hFF;
			enable_i = 1'b0;
		end
		#50	sel_i = 2'b00;
		#50	sel_i = 2'b01;
		#50	sel_i = 2'b10;
		#50	sel_i = 2'b11;
	end
	
	initial
	begin
		#400 $finish;
	end
	
endmodule
	