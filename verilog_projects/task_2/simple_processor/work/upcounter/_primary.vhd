library verilog;
use verilog.vl_types.all;
entity upcounter is
    port(
        Clear           : in     vl_logic;
        Clock           : in     vl_logic;
        En              : in     vl_logic;
        Q               : out    vl_logic_vector(1 downto 0)
    );
end upcounter;
