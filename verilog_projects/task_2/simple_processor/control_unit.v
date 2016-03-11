
module control_unit
	(
		input  wire [1:0] Tstep_Q,
		input  wire [8:0] IR,
		input  wire 		Rst,
		input  wire 		Run,
		
		output wire 		Clear,
		
		output reg 			AddSub,
		
		output reg	 		Ain,
		output reg	 		Gin,
		output reg	 		IRin,
		output reg  [7:0] Rin,
		
		output reg 			DINout,
		output reg			Gout,
		output reg  [7:0] Rout,
		
		output reg			En,
		output reg			Done
	);
	
	wire [7:0] Xreg, Yreg;
	wire [2:0] I;
	assign I = IR[8:6];
	
	assign Clear = Rst | Done;
	
	dec3to8 decX (IR[5:3], 1'b1, Xreg);
	dec3to8 decY (IR[2:0], 1'b1, Yreg);
	
	always @(Tstep_Q or I or Xreg or Yreg or Run)
	begin
	
		Ain = 1'b0; Gin = 1'b0; IRin = 1'b0; Rin = 8'd0;
		DINout = 1'b0; Gout = 1'b0; Rout = 8'd0;
		AddSub = 1'b0; Done = 1'b0;
		
		case (Tstep_Q)
		2'b00:
			begin
				IRin = (Run) ? 1'b1 : 1'b0;
				En = (Run) ? 1'b1 : 1'b0;
			end
		2'b01:
			begin
				case (I)
				3'b000: // move
					begin
						Rout = Xreg; Rin = Yreg; Done = 1'b1;
						En = 1'b0;
					end
				3'b001: // load
					begin
						DINout = 1'b1; Rin = Xreg; Done = 1'b1;
						En = 1'b0;
					end
				default: // add, sub
					begin
						Rout = Xreg; Ain = 1'b1;
					end
				endcase
			end
		2'b10:
			begin
				case (I)
				3'b010: // add
					begin
						Rout = Yreg; Gin = 1'b1;
					end
				3'b011: // sub
					begin
						Rout = Yreg; AddSub = 1'b1; Gin = 1'b1;
					end
				default: ;
				endcase
			end
		2'b11:
			begin
				case (I)
				3'b010, 3'b011:
					begin
						Gout = 1'b1; Rin = Xreg; Done = 1'b1;
						En = 1'b0;
					end
				default: ;
				endcase
			end
		endcase
	end
	
endmodule

	