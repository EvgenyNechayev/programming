library verilog;
use verilog.vl_types.all;
entity add_sub is
    generic(
        N               : integer := 16
    );
    port(
        A               : in     vl_logic_vector;
        B               : in     vl_logic_vector;
        Sub             : in     vl_logic;
        Q               : out    vl_logic_vector
    );
end add_sub;
