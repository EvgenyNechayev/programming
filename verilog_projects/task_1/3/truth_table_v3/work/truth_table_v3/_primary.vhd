library verilog;
use verilog.vl_types.all;
entity truth_table_v3 is
    port(
        a_i             : in     vl_logic_vector(3 downto 0);
        b_o             : out    vl_logic
    );
end truth_table_v3;
