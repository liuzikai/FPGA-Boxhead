module vga_color_extend (
    input logic [15:0] vga_data,
    output logic [7:0] VGA_R, VGA_G, VGA_B
);

    assign VGA_R = {vga_data[15:11], 3'b000};
    assign VGA_G = {vga_data[10:5], 2'b00};
    assign VGA_B = {vga_data[4:0], 3'b000};

endmodule