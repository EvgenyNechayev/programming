library verilog;
use verilog.vl_types.all;
entity d_trigger is
    port(
        rst_i           : in     vl_logic;
        clk_i           : in     vl_logic;
        en_i            : in     vl_logic;
        data_i          : in     vl_logic_vector(7 downto 0);
        data_o          : out    vl_logic_vector(7 downto 0)
    );
end d_trigger;
