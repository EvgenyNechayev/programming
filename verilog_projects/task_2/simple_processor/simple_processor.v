
module simple_processor
	#(parameter BITS = 16)
	(
		input  wire [BITS-1:0] DIN,
		input  wire 		 	  Resetn,
		input  wire 		  	  Clock,
		input  wire 		  	  Run,
		
		output wire				  Done,
		output wire [BITS-1:0] Bus
	);
	
	wire [BITS-1:0] BusWires;
	wire AddSub;
	wire Ain, Gin, IRin;
	wire Gout, DINout;
	wire [7:0] Rin, Rout;
	
	wire [BITS-1:0] R0, R1, R2, R3, R4, R5, R6, R7;
	wire [BITS-1:0] A, G;
	wire [8:0] IR;
	wire [BITS-1:0] Sum;
	wire [1:0] Tstep_Q;
	wire Clear, En;

	assign Bus = BusWires;
	
	mux_bus #(.N(BITS)) multiplexer 
		(
			.DINout(DINout), .Gout(Gout), .Rout(Rout),
			.R0(R0), .R1(R1), .R2(R2), .R3(R3), .R4(R4), .R5(R5), .R6(R6), .R7(R7), .G(G), .DIN(DIN),
			.BusOut(BusWires)
		);
	
	regn #(.N(BITS)) reg_0 (.R(BusWires), .Rin(Rin[0]), .Clock(Clock), .Rst(Resetn), .Q(R0));
	regn #(.N(BITS)) reg_1 (.R(BusWires), .Rin(Rin[1]), .Clock(Clock), .Rst(Resetn), .Q(R1));
	regn #(.N(BITS)) reg_2 (.R(BusWires), .Rin(Rin[2]), .Clock(Clock), .Rst(Resetn), .Q(R2));
	regn #(.N(BITS)) reg_3 (.R(BusWires), .Rin(Rin[3]), .Clock(Clock), .Rst(Resetn), .Q(R3));
	regn #(.N(BITS)) reg_4 (.R(BusWires), .Rin(Rin[4]), .Clock(Clock), .Rst(Resetn), .Q(R4));
	regn #(.N(BITS)) reg_5 (.R(BusWires), .Rin(Rin[5]), .Clock(Clock), .Rst(Resetn), .Q(R5));
	regn #(.N(BITS)) reg_6 (.R(BusWires), .Rin(Rin[6]), .Clock(Clock), .Rst(Resetn), .Q(R6));
	regn #(.N(BITS)) reg_7 (.R(BusWires), .Rin(Rin[7]), .Clock(Clock), .Rst(Resetn), .Q(R7));

	regn #(.N(BITS)) reg_A (.R(BusWires), .Rin(Ain), .Clock(Clock), .Rst(Resetn), .Q(A));
	regn #(.N(BITS)) reg_G (.R(Sum), .Rin(Gin), .Clock(Clock), .Rst(Resetn), .Q(G));
	regn #(.N(9))	  reg_IR (.R(DIN[BITS-1:7]), .Rin(IRin), .Clock(Clock), .Rst(Resetn), .Q(IR));
	
	upcounter counter (.Clear(Clear), .Clock(Clock), .En(En), .Q(Tstep_Q));

	add_sub #(.N(BITS)) AS (.A(A), .B(BusWires), .Sub(AddSub), .Q(Sum));
	
	control_unit CPU
		(
		.Tstep_Q(Tstep_Q), .IR(IR), .Rst(Resetn), .Run(Run),
		.Clear(Clear), .AddSub(AddSub),
		.Ain(Ain), .Gin(Gin), .IRin(IRin), .Rin(Rin),
		.DINout(DINout), .Gout(Gout), .Rout(Rout),
		.En(En),
		.Done(Done)
		);
	
endmodule
