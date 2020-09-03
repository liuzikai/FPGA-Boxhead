module snow_mem (
    input  logic clk,
    input  logic [13:0] read_addr,
    output logic [15:0] data_out
);
    
    logic [15:0] mem [0:9999];
    
    initial
    begin
         $readmemh("../resource/snow.txt", mem);
    end
    
    
    always_ff @ (posedge clk) begin
        data_out <= mem[read_addr];
    end
    
endmodule