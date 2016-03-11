
// Главный модуль 
module par_to_ser
	#(parameter PREAMBLE = 6,	// Преамбула
		    MAX_WORD = 5	// Максимально допустимое количество входных данных    
	)
	(
	input  wire 	  rst_i,	// Сигнал асинхронного сброса    
	input  wire 	  clk_i,	// Синхросигнал, по которому работает модуль    
	input  wire 	  en_i,		// Сигнал валидности входных данных   
	input  wire [7:0] prl_data_i,	// Восемь бит входных данных   

	output wire	  ser_data_o,	// Выход для последовательных данных   
	output wire	  en_o,		// Выход для подтверждения валидности передаваемых последовательных данных      
	output wire	  busy_o	// Сигнал занятости модуля
	);
	
	// Объявление внутренних wire
	wire [12:0] data_packet;	// Пакет данных - бит паритета + восемь бит данных + преамбула
	wire [3:0]  count;		// Счетчик - для проверки конца передачи пакета данных
	wire [7:0]  data;		// Байт данных с выхода управляющего блока
	wire load_shiftreg;		// Сигнал записи пакета данных в сдвиговый регистр
	wire en_shiftreg;		// Сигнал разрешения работы сдвигового регистра
	wire rst_counter;		// Сигнал сброса счетчика
	wire busy_module;		// Сигнал занятости модуля
	wire en_module;			// Сигнал для подтверждения валидности передаваемых последовательных данных
	// Непрерывные присваивания 
	assign en_o = en_module;
	assign busy_o = busy_module;
	assign data_packet = {^data, data, PREAMBLE[3:0]};
	// Объявление экземпляра упарвляющего блока
	control_block #(.N(MAX_WORD))	control_block_instance
		(.rst_i(rst_i), .clk_i(clk_i), .en_i(en_i), .data_i(prl_data_i), .count_i(count),
		 .load_shiftreg_o(load_shiftreg), .en_shiftreg_o(en_shiftreg), .en_o(en_module),
		 .rst_counter_o(rst_counter), .busy_o(busy_module), .data_o(data)
		);
	// Объявление экземпляра счетчика
	upcounter upcounter_instance
		(.rst_i(rst_counter), .clk_i(clk_i), .en_i(en_shiftreg),
		 .count_o(count)
		);
	// Объявление экземпляра сдвигового регистра
	lpm_shiftreg shiftreg_13bits
		(.clock(clk_i), .data(data_packet), .enable(en_shiftreg), .load(load_shiftreg),
		 .shiftout(ser_data_o)
		);
		defparam shiftreg_13bits.lpm_type = "LPM_SHIFTREG";
		defparam shiftreg_13bits.lpm_direction = "RIGHT";
		defparam shiftreg_13bits.lpm_width = 13;

endmodule
