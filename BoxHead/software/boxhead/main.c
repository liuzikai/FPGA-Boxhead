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

static const unsigned char DKEY_KEYCODE_1[4] = {
        26,  // W
        07,  // D
        22,  // S
        04   // A
};

static const unsigned char DKEY_KEYCODE_2[4] = {
        82,  // Up
        79,  // Right
        81,  // Down
        80   // Left
};

int keys_to_direction(const unsigned char dkey_pressed[]) {
    if (dkey_pressed[0] && dkey_pressed[1]) return 1;
    if (dkey_pressed[1] && dkey_pressed[2]) return 3;
    if (dkey_pressed[2] && dkey_pressed[3]) return 5;
    if (dkey_pressed[3] && dkey_pressed[0]) return 7;
    if (dkey_pressed[0]) return 0;
    if (dkey_pressed[1]) return 2;
    if (dkey_pressed[2]) return 4;
    if (dkey_pressed[3]) return 6;
    return -1;
}

int main() {

    keyboard_init();
    graphic_engine_reset();
    init_game();

    unsigned char keycode[8];

    unsigned int frame_count = 0;

    unsigned char dkey_pressed_1[4];  // {W, D, S, A}
    unsigned char dkey_pressed_2[4];  // {up, right, down, left}

    int direction_1, direction_2;
    int attack_1, attack_2;
    int should_update = 1;

    while (1) {

        if (keyboard_fetch((alt_u16 *) (&keycode)) != 0) {
            // Failed to fetch keycode
            for (int i = 0; i < 8; i++) {
                keycode[i] = 0;
            }
            printf("FAILED TO FETCH KEYCODE!\n");
        }

        // Extract keys
        for (int key = 0; key < 4; key++) {
            dkey_pressed_1[key] = dkey_pressed_2[key] = 0;
            for (int i = 0; i < 8; i++) {
                if (keycode[i] == 21) {
                    init_game();
                    should_update = 1;
                }
                if (keycode[i] == DKEY_KEYCODE_1[key]) {
                    dkey_pressed_1[key] = 1;
                }
                if (keycode[i] == DKEY_KEYCODE_2[key]) {
                    dkey_pressed_2[key] = 1;
                }
            }
        }
        direction_1 = keys_to_direction(dkey_pressed_1);
        direction_2 = keys_to_direction(dkey_pressed_2);

        attack_1 = attack_2 = 0;
        for (int i = 0; i < 8; i++) {
            if (keycode[i] == 44) {  // Space
                attack_1 = 1;
            }
            if (keycode[i] == 40) {  // Enter
                attack_2 = 1;
            }
        }
        
        should_update = refresh(should_update, direction_1, direction_2, attack_1, attack_2);

        wait_for_next_frame();
    }

    return 0;
}

