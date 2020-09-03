/*
    Graphic Copy Engine

    Use 0x07E0 (pure green) as transparent color
*/

module copy_engine #(SrcAddrWidth) (
    input  logic                      clk,         // 50 MHz clock
    input  logic                      reset,       // active-high reset signal

    // Control signals
    input  logic [9:0]                dest_x_start,
                                      dest_x_end,  // exclusive
                                      dest_y_start,
                                      dest_y_end,  // exclusive
    input  logic [SrcAddrWidth-1:0]   src_addr_start,
    input  logic                      execute,

    // Output status
    output logic                      status,

    // Wires between data src
    output logic [SrcAddrWidth-1:0]   src_addr,
    input  logic [15:0]               src_data,

    // Data to SRAM controller
    output logic [9:0]                program_x,
                                      program_y,
    output logic [15:0]               program_data,
    output logic                      program_write
);   

    enum logic [1:0] {
        STATE_FREE,
        STATE_RUNNING,
        STATE_DONE
    } state, state_in;

    logic [9:0] x, y;
    logic [9:0] x_in, y_in;

    logic [SrcAddrWidth-1:0] src_addr_reg, src_addr_reg_next;

    always_ff @ (posedge clk)
	begin
		if (reset) begin
			state <= STATE_FREE;
            x <= 10'b0;
            y <= 10'b0;
            src_addr_reg <= {SrcAddrWidth{1'b0}};
        end else begin
			state <= state_in;
            x <= x_in;
            y <= y_in;
            src_addr_reg <= src_addr_reg_next;
		end
	end

    always_comb begin
        unique case (state)
            STATE_FREE: begin
                state_in = execute ? STATE_RUNNING : STATE_FREE;
                x_in = dest_x_start;
                y_in = dest_y_start;
                src_addr_reg_next = src_addr_start;
            end
            STATE_RUNNING: begin
                state_in = STATE_RUNNING;  // default
                x_in = x + 10'd1;
                y_in = y;
                if (x + 10'd1 == dest_x_end) begin
                    x_in = dest_x_start;
                    y_in = y + 10'd1;
                    if (y + 10'd1 == dest_y_end) begin
                        y_in = y;
                        state_in = STATE_DONE;  // done
                    end
                end
                if (execute == 1'b0) state_in = STATE_FREE;  // early exit
                src_addr_reg_next = src_addr_reg + 1;
            end
            STATE_DONE: begin
                state_in = execute ? STATE_DONE : STATE_FREE;
                x_in = x;
                y_in = y;
                src_addr_reg_next = src_addr_start;
            end
        endcase
    end

    assign status = (state != STATE_DONE);
    assign program_x = x;
    assign program_y = y;
    assign src_addr = src_addr_reg_next;  // request next data, which will be available at next cycle
    assign program_data = src_data;
    assign program_write = (state == STATE_RUNNING && program_data != 16'h07E0);

endmodule