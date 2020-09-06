#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <unistd.h>

#include "keyboard/keyboard.h"
#include "graphic/graphic_engine.h"

int main() {

//    keyboard_init();
	graphic_engine_reset();

    unsigned int keycode;

    int frame_index = 0;
    int frame_count = 0;

    while (1) {
        draw(320 - 18, 320 + 18, 240 - 19, 240 + 20, frame_index * 1404, 0, 1);
//        printf("!\n");

//        if (keyboard_fetch(&keycode) != 0) {  // failed to fetch keycode
//            keycode = 0;
//            if (keyboard_hot_plugged()) {
//                keyboard_init();
//            }
//        }

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

