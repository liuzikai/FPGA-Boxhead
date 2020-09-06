#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <unistd.h>

#include "keyboard/keyboard.h"

int main(void) {

    keyboard_init();

    unsigned int keycode;
    bool keyboard_fetch_succeed;

    while (1) {
        if (keyboard_fetch(&keycode) != 0) {  // failed to fetch keycode
            keycode = 0;
            if (keyboard_hot_plugged()) {
                keyboard_init();
            }
        }

        
    }

    return 0;
}

