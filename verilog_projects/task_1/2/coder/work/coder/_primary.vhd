library verilog;
use verilog.vl_types.all;
entity coder is
    generic(
        N               : integer := 1024;
        M               : integer := 10
    );
    port(
        data_i          : in     vl_logic_vector;
        enable_i        : in     vl_logic;
        data_o          : out    vl_logic_vector
    );
end coder;
