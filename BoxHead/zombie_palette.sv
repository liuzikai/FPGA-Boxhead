module zombie_palette (
    input  logic [3:0]  index,
    output logic [15:0] color
);

    always_comb begin
        unique case (index)
            0: color = 16'h8410;
            1: color = 16'hBDD7;
            2: color = 16'hBE9D;
            3: color = 16'h8C51;
            4: color = 16'h5B2E;
            5: color = 16'h18E3;
            6: color = 16'h9CD3;
            7: color = 16'h18C3;
            8: color = 16'h10A2;
            9: color = 16'h1082;
           10: color = 16'h2104;
           11: color = 16'hBDF7;
           12: color = 16'hFFFF;
           13: color = 16'h8430;
           14: color = 16'h2124;
           15: color = 16'h07E0;
       endcase
    end

endmodule