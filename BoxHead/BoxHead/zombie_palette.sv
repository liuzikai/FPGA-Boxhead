module zombie_palette (
    input  logic [2:0]  index,
    output logic [15:0] color
);

    always_comb begin
        unique case (index)
            0: color = 16'h8430;
            1: color = 16'hFFFF;
            2: color = 16'hBE9D;
            3: color = 16'h5B4E;
            4: color = 16'h1082;
            5: color = 16'h2124;
            6: color = 16'h738E;
            7: color = 16'h07E0;
        endcase
    end

endmodule