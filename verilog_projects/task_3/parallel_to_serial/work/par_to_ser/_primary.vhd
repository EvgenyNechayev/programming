library verilog;
use verilog.vl_types.all;
entity par_to_ser is
    generic(
        PREAMBLE        : integer := 6;
        MAX_WORD        : integer := 5
    );
    port(
        rst_i           : in     vl_logic;
        clk_i           : in     vl_logic;
        en_i            : in     vl_logic;
        prl_data_i      : in     vl_logic_vector(7 downto 0);
        ser_data_o      : out    vl_logic;
        en_o            : out    vl_logic;
        busy_o          : out    vl_logic
    );
end par_to_ser;
