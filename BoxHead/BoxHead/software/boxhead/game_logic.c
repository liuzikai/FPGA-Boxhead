#include "game_logic.h"
#include "resource.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef enum {
    STATIC,
    MOVE,
    HIT_BACK,
    HIT_FRONT,
    ATTACK, 
    DIE
} movement_t;

// Structure of the zombies
struct Zombie {
    int index; // TODO: Index used for sorting
    int x_pos; // Current x position of the zombie
    int y_pos; // Current y position of the zombie
    int blood; // Current HP of the zombie
    int enable; // Whether the zombie is alive
    movement_t movement;
    int direction; // TODO
    int frame; // TODO
};

// Structure of the player
struct Player {
    int x_pos; // Current x position of the zombie
    int y_pos; // Current y position of the zombie
    int blood; // Current HP of the zombie
    int direction; // Current direction of the player
    int enable; // Whether the player is alive
    int credit; // Credit for the player
    movement_t movement;
    int frame; // TODO
};

// Global variables
static int display[WINDOW_WIDTH][WINDOW_HEIGHT];  // index

static struct Zombie zombies[MAX_ZOMBIES_ON_SCREEN]; 
static struct Player player_1, player_2;

static int time_counter = 0;
static int zombie_appear_counter = 0;
static int cur_zombie_counter = 0;

/**
 * Helper functions
 */

/**
 * This function determines the position of the zombies
 * Directions about the zombies
 * 7 8 9
 * 4 5 6 
 * 1 2 3
 */

char determine_pos(int delta_x, int delta_y) {
    if (delta_x == 0) {
        if (delta_y == 0) {
            return '5';
        } else if (delta_y > 0) {
            return '2';
        } else {
            return '8';
        }
    } else if (delta_y == 0) {
        if (delta_x > 0) {
            return '6';
        } else {
            return '4';
        }
    } else {
        if (delta_x < 0) {
            if (delta_y > 0) {
                return '1';
            } else {
                return '7';
            }
        } else {
            if (delta_y > 0) {
                return '3';
            } else {
                return '9';
            }
        }
    }
}

/**
 * This helper function is used to determine whether the attack from zombie to player is from back or front
 * Also update the frame for player
 */

void zombie_attack(struct Zombie * cur_zombie, struct Player * cur_player) {
    cur_player->blood -= ZOMBIE_ATTACT;
    if (cur_zombie->direction == cur_player->direction) {  // hit back
        cur_player->direction = HIT_BACK;
        cur_player->frame = 0;
    } else {  // hit front
        cur_player->direction = HIT_FRONT;
        cur_player->frame = 0;
    }
}

int create_zombie(struct Zombie* cur_zombie) {
    if (cur_zombie->enable == 0) {
        // Check whether there is zombie at the born place
        if (display[WINDOW_WIDTH / 2][0] != -1){
            return 1;
        } else {
            cur_zombie->x_pos = WINDOW_WIDTH / 2;
            cur_zombie->y_pos = 0;
            cur_zombie->enable = 1;
            cur_zombie->blood = 100; // Current HP of the zombie
            cur_zombie->direction = 2; 
            cur_zombie->movement = MOVE; 
            cur_zombie->frame = 0; 
            display[WINDOW_WIDTH / 2][0] = cur_zombie->index;
            return 0;
        }
    }
}

void step_zombie(struct Zombie* cur_zombie) {
    int dx, dy;
    int new_x, new_y;
    switch (cur_zombie->direction) {
        case 3:
        case 6:
        case 9:
            dx = 1;
            break;
        case 2:
        case 8:
            dx = 0;
            break;
        case 1:
        case 4:
        case 7:
            dx = -1
            break;
    }
    switch (cur_zombie->direction) {
        case 7:
        case 8:
        case 9:
            dy = 1;
            break;
        case 4:
        case 6:
            dy = 0;
            break;
        case 1:
        case 2:
        case 3:
            dy = -1
            break;
    }

    new_x = cur_zombie->x_pos + dx;
    new_y = cur_zombie->x_pos + dy;

    // Check for window boarders
    if (new_x >= WINDOW_WIDTH) {
        new_x = WINDOW_WIDTH - 1;
    } else if (new_x < 0) {
        new_x = 0
    }
    if (new_y >= WINDOW_HEIGHT) {
        new_y = WINDOW_HEIGHT - 1;
    } else if (new_y < 0) {
        new_y = 0
    }

    // Check for collisions
    if (display[new_x][new_y] == -1) {
        display[cur_zombie->x_pos][cur_zombie->y_pos] = -1;
        display[new_x][new_y] = cur_zombie->index;
        cur_zombie->x_pos = new_x;
        cur_zombie->y_pos = new_y;
    }
}

void change_zombie_direction(struct Zombie* cur_zombie) {
    int delta_x, delta_y;
    int delta_x1, delta_y1, delta_x2, delta_y2;
    int closer;
    float normalizer; 
    
    // Check for existing zombies
    // All delta distances are the relevant distance with respect to the player

    delta_x1 = player_1.x_pos - cur_zombie->x_pos;
    delta_y1 = player_1.y_pos - cur_zombie->y_pos;
    delta_x2 = player_2.x_pos - cur_zombie->x_pos;
    delta_y2 = player_2.y_pos - cur_zombie->y_pos;

    if (player_1.enable == 0) {
        closer = 2;
    } else if (player_2.enable == 0) {
        closer = 1;
    } else {
        if (delta_x1 * delta_x1 + delta_y1 * delta_y1 < delta_x2 * delta_x2 + delta_y2 * delta_y2) { // closer to player_1
            closer = 1;
        } else {
            closer = 2;
        }
    }

    if (closer == 1) {
        delta_x = delta_x1;
        delta_y = delta_y1;
    } else {
        delta_x = delta_x2;
        delta_y = delta_y2;
    }
    
    cur_zombie->direction = (int) (determine_pos(delta_x, delta_y) - '0');
}

/**
 * This function checks for collision for player move 
 * as well as cause damage to the zombies
 * Return 0 for success and return 1 for failure.
 * Directions are defined the same as above. 
 * 8 - up; 4 - left; 6 - right; 2 - down
 */

int move_player(struct Player* player_1, int direction){
    int delta_x = 0;
    int delta_y = 0;
    int new_x, new_y;
    switch (direction) {
        case 8: // up
            delta_y = -PLAYER_MOVE;
            break;
        case 2: // down
            delta_y = PLAYER_MOVE;
            break;
        case 4: // left
            delta_x = -PLAYER_MOVE;
            break;
        case 6: // right
            delta_x = PLAYER_MOVE;
            break;
        default:
            break;
    }

    if (player_1.movement == 1) {
        if (player_1.direction == direction) {
            player_1.frame++;
            if (player_1.frame == PLAYER_WALK_FRAME_COUNT) {
                player_1.frame = 0;
            }
        }
    } else {
        player_1.movement = 1;
        player_1.frame = 0;
    }
    
    // Check for boundaries
    if (player_1.x_pos + delta_x < 0) {
        new_x = 0;
    } else if (player_1.x_pos + delta_x >= WINDOW_WIDTH) {
        new_x = WINDOW_WIDTH - 1;
    } else {
        new_x = player_1.x_pos + delta_x;
    }
    if (player_1.y_pos + delta_y < 0) {
        new_y = 0;
    } else if (player_1.y_pos + delta_y >= WINDOW_HEIGHT) {
        new_y = WINDOW_HEIGHT - 1;
    } else {
        new_y = player_1.y_pos + delta_y;
    }  

    if (display[new_x][new_y] == ' ') {
        display[player_1.x_pos][player_1.y_pos] = ' ';
        display[new_x][new_y] = '*';
        player_1.x_pos = new_x;
        player_1.y_pos = new_y;
    } else {
        return 1;
    }
    return 0;
}

/**
 * This helper function is used to determine whether the attack from player to zombie is from back or front
 * Also update the frame for zombies
 */
void player_attack_judge(struct Zombie * zombies, int i, struct Player * cur_player) {
    zombies[i].blood -= PLAYER_ATTACT;
    if (zombies[i].direction == cur_player->direction) { // hit back
        if (zombies[i].movement != 2) {
            zombies[i].movement = 2;// 0 - static, 1 - move, 2 - hit back, 3 - hit front, 4 - die
            zombies[i].frame = 0;
        } else {
            if (zombies[i].frame == ZOMBIE_HIT_BACK_FRAME_COUNT) {
                zombies[i].frame = 0;
            } else {
                zombies[i].frame++;
            }
        }
    } else { // hit front
        if (zombies[i].movement != 3) {
            zombies[i].movement = 3;// 0 - static, 1 - move, 2 - hit back, 3 - hit front, 4 - die
            zombies[i].frame = 0;
        } else {
            if (zombies[i].frame == ZOMBIE_HIT_FRONT_FRAME_COUNT) {
                zombies[i].frame = 0;
            } else {
                zombies[i].frame++;
            }
        }
    }
}

int player_attack (struct Player * player_1, struct Zombie * zombies) {
    switch (player_1.direction) {
        case 8: // up
            for (int i = 0;  i < MAX_ZOMBIES_ON_SCREEN; i++) {
                if ((zombies[i].enable == 1) && (abs(zombies[i].x_pos - player_1.x_pos) < LASER_RANGE) && (zombies[i].y_pos < player_1.y_pos)){
                    player_attack_judge(zombies, i, player_1);
                    player_1.credit ++;
                }
            }
        break;
        case 4: // left
            for (int i = 0;  i < MAX_ZOMBIES_ON_SCREEN; i++) {
                if ((zombies[i].enable == 1) && (abs(zombies[i].y_pos - player_1.y_pos) < LASER_RANGE) && (zombies[i].x_pos < player_1.x_pos)){
                    player_attack_judge(zombies, i, player_1);
                    player_1.credit ++;
                }
            }
        break;
        case 2: // down
            for (int i = 0;  i < MAX_ZOMBIES_ON_SCREEN; i++) {
                if ((zombies[i].enable == 1) && (abs (zombies[i].x_pos - player_1.x_pos) < LASER_RANGE) && (zombies[i].y_pos > player_1.y_pos)){
                    player_attack_judge(zombies, i, player_1);
                    player_1.credit ++;
                }
            }
        break;
        case 6: // right
            for (int i = 0;  i < MAX_ZOMBIES_ON_SCREEN; i++) {
                if ((zombies[i].enable == 1) && (abs (zombies[i].y_pos - player_1.y_pos) < LASER_RANGE) && (zombies[i].x_pos > player_1.x_pos)){
                    player_attack_judge(zombies, i, player_1);
                    player_1.credit ++;
                }
            }
        break;
        default:
        break;
    }
}

/**
 * The initialization function for the game logic. Run before calling the draw frame logic
 */

int initialization() {
    // Initialization
    // Players are born at the center of the screen
    int i, j;
    for (i = 0; i < WINDOW_WIDTH; i++){
        for (j = 0; j < WINDOW_HEIGHT; j++){
            display[i][j] = ' ';
        }
    }
    player_1.x_pos = WINDOW_WIDTH / 3;
    player_1.y_pos = WINDOW_HEIGHT / 2;
    player_1.blood = 100;
    display[player_1.x_pos][player_1.y_pos] = '*';
    player_1.enable = 1;
    player_1.credit = 0;
    player_1.movement = 0;
    player_1.direction = 5;
    player_1.frame = 0;

    player_2.x_pos = WINDOW_WIDTH * 2 / 3;
    player_2.y_pos = WINDOW_HEIGHT / 2;
    player_2.blood = 100;
    display[player_2.x_pos][player_2.y_pos] = '*';
    player_2.enable = 1;
    player_2.credit = 0;
    player_2.movement = 0;
    player_2.direction = 5;
    player_2.frame = 0;

    // Zombies are born at the the top center of the screen
    zombies[0].x_pos = WINDOW_WIDTH / 2;
    zombies[0].y_pos = 0;
    zombies[0].blood = 100;
    zombies[0].enable = 1;
    display[zombies[0].x_pos][zombies[0].y_pos] = '2';
    cur_zombie_counter ++;
    zombies[0].direction = 2;
    zombies[0].frame = 0;

}

/**
 * Main function of the game logic
 */

int draw_one_frame(int direction_1, int direction_2, int attack_1, int attack_2) {

    /**
     *  Unit test display tool
     * ' ' means empty
     * 
     * Directions about the zombies
     * 7 8 9
     * 4 5 6 
     * 1 2 3
     * 
     * '*' means the position of the player
     */

    // Unit test print
    int i, j;
    for (j = 0; j < WINDOW_HEIGHT; j++) {
        for (i = 0; i < WINDOW_WIDTH; i++) {
            printf("%c", display[i][j]);
        }
        printf("\n");
    }
    printf("Player 1 blood: %d Player 2 blood: %d\n", player_1.blood, player_2.blood);
    printf("Player 1 credit: %d Player 2 credit: %d\n", player_1.credit, player_2.credit);
    printf("Current time: %d\n", time_counter);

    // Move all the zombies
    
    for (i = 0; i < MAX_ZOMBIES_ON_SCREEN; i++) {
        if (zombies[i].enable == 1) {
            move_zombie(display, &(zombies[i]), &player_1, &player_2);
        }
    }
    
    
    // Add new zombies
    time_counter ++;
    if (time_counter % 4 == 0) {
        if ((zombie_appear_counter < MAX_TOTAL_ZOMBIES) && (cur_zombie_counter < MAX_ZOMBIES_ON_SCREEN)) {
            for (i = 0; i < MAX_ZOMBIES_ON_SCREEN; i++){
                if (zombies[i].enable == 0){
                    zombies[i].index = cur_zombie_counter;
                    zombies[i].blood = 100;
                    if (0 == move_zombie(display, &(zombies[i]), &player_1, &player_2)) {
                        cur_zombie_counter ++;
                        zombie_appear_counter ++;
                        break;
                    } 
                }
            }
        }
    }

    

    // Check for user input and update player position
    // Update blood of the zombies
    if (player_1.enable == 1) {
        switch (direction_1) {
            case 8: // up
                move_player(display, zombies, &player_1, 8);
            break;
            case 4: // left
                move_player(display, zombies, &player_1, 4);
            break;
            case 2: // down
                move_player(display, zombies, &player_1, 2);
            break;
            case 6: // right
                move_player(display, zombies, &player_1, 6);
            break;
            default:
            break;
        }

        if (attack_1 == 1) {
            player_attack(&player_1, zombies);
        }
    }
        
        // Player 2
    if (player_2.enable == 1) {
        switch (direction_2) {
            case 'i': // up
                move_player(display, zombies, &player_2, 8);
            break;
            case 'j': // left
                move_player(display, zombies, &player_2, 4);
            break;
            case 'k': // down
                move_player(display, zombies, &player_2, 2);
            break;
            case 'l': // right
                move_player(display, zombies, &player_2, 6);
            break;
        }
        
        if (attack_2 == 1) {
            player_attack(&player_2, zombies);
        } 
    }    
    

    // Check for death of zombies and the player
    // TODO: Use enable to determine whether need to display
    // Use movement to replace enable here
    if (player_1.blood <= 0) {
        if (player_1.enable == 1) {  // just been dead
            player_1.enable = 0;
            player_1.movement = 4;
            player_1.frame = 0;
        } else {
            if (player_1.frame == PLAYER_DIE_FRAME_COUNT) {
                printf("Player 1 is dead!");
                display[player_1.x_pos][player_1.y_pos] = ' ';
                player_1.frame = 0;
            } else if (player_1.frame != 0){
                player_1.frame++;
            } else {
                ;
            }
        }       
    } 
    if (player_2.blood <= 0) {
        if (player_2.enable == 1) {  // just been dead
            player_2.enable = 0;
            player_2.movement = 4;
            player_2.frame = 0;
        } else {
            if (player_2.frame == PLAYER_DIE_FRAME_COUNT) {
                printf("Player 2 is dead!");
                display[player_2.x_pos][player_2.y_pos] = ' ';
                player_2.frame = 0;
            } else if (player_2.frame != 0){
                player_2.frame++;
            } else {
                ;
            }
        }
    } 
    if ((player_1.enable == 0) && (player_2.enable == 0)) {
        printf( "You are all dead, please restart the game!");
        return 0;
    }
    if ((cur_zombie_counter == 0) && (zombie_appear_counter == MAX_TOTAL_ZOMBIES)) {
        printf("Congratulations, you have killed all the zombies!");
        return 0;
    }
    for (i = 0; i < MAX_ZOMBIES_ON_SCREEN; i++) {
        if ((zombies[i].enable == 1) && (zombies[i].blood <= 0)) {
            display[zombies[i].x_pos][zombies[i].y_pos] = ' '; // bug: forgot to erase the dead zombie
            zombies[i].enable = 0;
            cur_zombie_counter --;
        }
    }

    

    return 0;
}
