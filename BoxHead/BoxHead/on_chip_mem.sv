module on_chip_mem (
    input  logic clk,
    input  logic [21:0] read_addr,
    output logic [2:0] data_out
);
    
    logic [2:0] mem [0:1000000];
    
    initial
    begin
         $readmemh("resource/zombie_walk_out/output.txt", mem);
    end
    
    
    always_ff @ (posedge clk) begin
        data_out <= mem[read_addr];
    end
    
endmodule