library verilog;
use verilog.vl_types.all;
entity control_unit is
    port(
        Tstep_Q         : in     vl_logic_vector(1 downto 0);
        IR              : in     vl_logic_vector(8 downto 0);
        Rst             : in     vl_logic;
        Run             : in     vl_logic;
        Clear           : out    vl_logic;
        AddSub          : out    vl_logic;
        Ain             : out    vl_logic;
        Gin             : out    vl_logic;
        IRin            : out    vl_logic;
        Rin             : out    vl_logic_vector(7 downto 0);
        DINout          : out    vl_logic;
        Gout            : out    vl_logic;
        Rout            : out    vl_logic_vector(7 downto 0);
        En              : out    vl_logic;
        Done            : out    vl_logic
    );
end control_unit;
