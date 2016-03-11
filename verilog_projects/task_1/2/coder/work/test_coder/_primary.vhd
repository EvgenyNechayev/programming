library verilog;
use verilog.vl_types.all;
entity test_coder is
    generic(
        N               : integer := 1024;
        M               : integer := 10
    );
end test_coder;
