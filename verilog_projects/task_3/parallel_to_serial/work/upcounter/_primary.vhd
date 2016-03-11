library verilog;
use verilog.vl_types.all;
entity upcounter is
    port(
        rst_i           : in     vl_logic;
        clk_i           : in     vl_logic;
        en_i            : in     vl_logic;
        count_o         : out    vl_logic_vector(3 downto 0)
    );
end upcounter;
