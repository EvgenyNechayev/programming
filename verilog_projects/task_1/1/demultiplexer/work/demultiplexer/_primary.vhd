library verilog;
use verilog.vl_types.all;
entity demultiplexer is
    port(
        data_i          : in     vl_logic_vector(7 downto 0);
        sel_i           : in     vl_logic_vector(1 downto 0);
        enable_i        : in     vl_logic;
        data_a_o        : out    vl_logic_vector(7 downto 0);
        data_b_o        : out    vl_logic_vector(7 downto 0);
        data_c_o        : out    vl_logic_vector(7 downto 0);
        data_d_o        : out    vl_logic_vector(7 downto 0)
    );
end demultiplexer;
