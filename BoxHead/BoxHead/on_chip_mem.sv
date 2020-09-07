module on_chip_mem (
    input  logic clk,
    input  logic [19:0] read_addr,
    output logic [3:0] data_out
);

    logic [15:0] raw_data;
    on_chip_mem_core on_chip_mem_core (
        .clk(clk),
        .read_addr(read_addr[19:2]),
        .data_out(raw_data)
    );

    always_comb begin
        unique case (read_addr[1:0])
            2'b00: data_out = raw_data[3:0];
            2'b01: data_out = raw_data[7:4];
            2'b10: data_out = raw_data[11:8];
            2'b11: data_out = raw_data[15:12];
        endcase
    end

endmodule

module on_chip_mem_core (
    input  logic clk,
    input  logic [17:0] read_addr,
    output logic [15:0] data_out
);
    
    logic [15:0] mem [0:235532];
    
    initial
    begin
         $readmemh("resource/BoxHead/ocm.txt", mem);
    end
    
    
    always_ff @ (posedge clk) begin
        data_out <= mem[read_addr];
    end
    
endmodule