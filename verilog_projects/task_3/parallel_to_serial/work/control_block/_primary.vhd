library verilog;
use verilog.vl_types.all;
entity control_block is
    generic(
        N               : integer := 5
    );
    port(
        rst_i           : in     vl_logic;
        clk_i           : in     vl_logic;
        en_i            : in     vl_logic;
        data_i          : in     vl_logic_vector(7 downto 0);
        count_i         : in     vl_logic_vector(3 downto 0);
        load_shiftreg_o : out    vl_logic;
        en_o            : out    vl_logic;
        en_shiftreg_o   : out    vl_logic;
        rst_counter_o   : out    vl_logic;
        busy_o          : out    vl_logic;
        data_o          : out    vl_logic_vector(7 downto 0)
    );
end control_block;
