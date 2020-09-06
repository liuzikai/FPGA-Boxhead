#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <unistd.h>

#include "keyboard/keyboard.h"
#include "graphic/graphic_engine.h"

int main(void) {

    keyboard_init();

    unsigned int keycode;
    bool keyboard_fetch_succeed;

    int last_direction;

    while (1) {
        draw(320 - 30, 320 + 30, 240 - 33, 240 + 33, 0, 0, 0);

        if (keyboard_fetch(&keycode) != 0) {  // failed to fetch keycode
            keycode = 0;
            if (keyboard_hot_plugged()) {
                keyboard_init();
            }
        }

        wait_for_next_frame();
    }

    return 0;
}

