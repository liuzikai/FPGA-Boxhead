//
// Created by liuzikai on 9/6/20.
//

#include "graphic_engine.h"

#include "system.h"

volatile unsigned int *ENGINE_REGS = (unsigned int *) (0x40);

void graphic_engine_reset() {
	ENGINE_REGS[7] = 0;
	while ((ENGINE_REGS[15] & 0x1) != 0) {}  // wait for reset
}

void draw(int start_x, int end_x, int start_y, int end_y, int src_offset, int palette, int flip_x) {

    // Wait for the copy engine to finish last task
//	printf("Start waiting for graphic engine...\n");
	if (ENGINE_REGS[7] == 1) {
		while ((ENGINE_REGS[15] & 0x1) == 0) {}

		// Clear Execute
		ENGINE_REGS[7] = 0;
		while ((ENGINE_REGS[15] & 0x1) != 0) {}  // wait for reset
	}
//    printf("Graphic engine done!\n");

    ENGINE_REGS[0] = start_x;
    ENGINE_REGS[1] = end_x;
    ENGINE_REGS[2] = start_y;
    ENGINE_REGS[3] = end_y;
    ENGINE_REGS[4] = src_offset;
    ENGINE_REGS[5] = palette;
    ENGINE_REGS[6] = flip_x;

    // Execute
    ENGINE_REGS[7] = 1;

    // Let the engine run and C program can do other things
}

void wait_for_next_frame() {

    static unsigned int current_frame = 0;

    // Wait for switch to next frame
    while (current_frame == ENGINE_REGS[14]) {}

    current_frame = ENGINE_REGS[14];
}
