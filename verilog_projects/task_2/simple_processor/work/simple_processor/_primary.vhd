library verilog;
use verilog.vl_types.all;
entity simple_processor is
    generic(
        BITS            : integer := 16
    );
    port(
        DIN             : in     vl_logic_vector;
        Resetn          : in     vl_logic;
        Clock           : in     vl_logic;
        Run             : in     vl_logic;
        Done            : out    vl_logic;
        \Bus\           : out    vl_logic_vector
    );
end simple_processor;
