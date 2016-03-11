library verilog;
use verilog.vl_types.all;
entity counter is
    port(
        rst_i           : in     vl_logic;
        clk_i           : in     vl_logic;
        en_i            : in     vl_logic;
        data_o          : out    vl_logic_vector(3 downto 0)
    );
end counter;
