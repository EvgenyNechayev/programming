library verilog;
use verilog.vl_types.all;
entity mux_bus is
    generic(
        N               : integer := 16
    );
    port(
        DINout          : in     vl_logic;
        Gout            : in     vl_logic;
        Rout            : in     vl_logic_vector(7 downto 0);
        R0              : in     vl_logic_vector;
        R1              : in     vl_logic_vector;
        R2              : in     vl_logic_vector;
        R3              : in     vl_logic_vector;
        R4              : in     vl_logic_vector;
        R5              : in     vl_logic_vector;
        R6              : in     vl_logic_vector;
        R7              : in     vl_logic_vector;
        G               : in     vl_logic_vector;
        DIN             : in     vl_logic_vector;
        BusOut          : out    vl_logic_vector
    );
end mux_bus;
