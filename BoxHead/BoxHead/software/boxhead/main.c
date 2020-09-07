#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <unistd.h>

#include "keyboard/keyboard.h"
#include "graphic/graphic_engine.h"
#include "graphic/resource.h"
#include "game_logic.h"

static const unsigned char dkey_keycode[4] = {
    26,  // W
    07,  // D
    22,  // S
    04   // A
};

int main() {

    keyboard_init();
	graphic_engine_reset();

    alt_u16 keycode;

    unsigned int frame_index = 0;
    unsigned int frame_count = 0;

    alt_u8 dkey_pressed[4];  // {up, right, down, left}

    unsigned int direction = 0;
    unsigned int width, height, half_width, half_height;

    while (1) {

        width = zombie_width[direction];
        height = zombie_height[direction];
        half_width = width / 2;
        half_height = height / 2;

        draw(320 - half_width, 
             320 + (width - half_width), 
             240 - half_height, 
             240 + (height - half_height), 
             zombie_offset[direction] + frame_index * width * height,
             0, 
             zombie_flip_x[direction]);

        if (keyboard_fetch(&keycode) != 0) {  // failed to fetch keycode
            keycode = 0;
//            if (keyboard_hot_plugged()) {
//                keyboard_init();
//            }
        }

        // Extract keys
        for (int key = 0; key < 4; key++) {
            dkey_pressed[key] = 0;
            for (int i = 0; i < 2; i++) {
                if (((keycode >> (8 * i)) & 0xFFU) == dkey_keycode[key]) {
                    dkey_pressed[key] = 1;
                }
            }
        }

        printf("%d, %d, %d, %d\n", dkey_pressed[0], dkey_pressed[1], dkey_pressed[2], dkey_pressed[3]);

        for (int key = 0; key < 4; key++) {
        	if (dkey_pressed[key] == 1 && dkey_pressed[(key + 1) % 4] == 1) {
				direction = key * 2 + 1;
				break;
			}
            if (dkey_pressed[key] == 1 && dkey_pressed[(key + 1) % 4] == 0) {
                direction = key * 2;
                break;
            }
        }

        printf("%d\n", direction);

        ++frame_count;
        if (frame_count == 8) {
        	frame_count = 0;
        	++frame_index;
        	if (frame_index == 8) frame_index = 0;
        }

        wait_for_next_frame();
    }

    return 0;
}

