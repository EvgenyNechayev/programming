library verilog;
use verilog.vl_types.all;
entity regn is
    generic(
        N               : integer := 16
    );
    port(
        R               : in     vl_logic_vector;
        Rin             : in     vl_logic;
        Clock           : in     vl_logic;
        Rst             : in     vl_logic;
        Q               : out    vl_logic_vector
    );
end regn;
