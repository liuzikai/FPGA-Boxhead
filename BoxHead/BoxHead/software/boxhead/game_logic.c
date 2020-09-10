#include <stdio.h>
#include <stdlib.h>

#include "game_logic.h"
#include "graphic/resource.h"
#include "graphic/graphic_engine.h"

#define DISPLAY_WIDTH  640
#define DISPLAY_HEIGHT 480
#define GRID_SIZE      32
#define GRID_X_COUNT   (DISPLAY_WIDTH / GRID_SIZE)
#define GRID_Y_COUNT   (DISPLAY_HEIGHT / GRID_SIZE)
#define TO_GRID(n) ((n) / GRID_SIZE)

#define MIN_X  32
#define MAX_X  608
#define MIN_Y  32
#define MAX_Y  448

#define PLAYER_1_BORN_X (DISPLAY_WIDTH / 3)
#define PLAYER_1_BORN_Y (DISPLAY_HEIGHT / 2)
#define PLAYER_2_BORN_X (DISPLAY_WIDTH / 3 * 2)
#define PLAYER_2_BORN_Y (DISPLAY_HEIGHT / 2)
#define ZOMBIE_BORN_X (DISPLAY_WIDTH / 2)
#define ZOMBIE_BORN_Y (MIN_Y)

#define MAX_TOTAL_ZOMBIES 100     // maximum number of zombies a player need to kill in one game
#define MAX_ZOMBIES_ON_SCREEN 10  // maximum number of zombies that can appear on the screen

#define ZOMBIE_MOVE 1  // moving speed for zombies
#define PLAYER_MOVE 6  // moving speed for player

#define ZOMBIE_ATTACK 10  // HP decrease when a zombie is close to the player
#define PLAYER_ATTACK 34  // HP decrease when the player shoots at a zombie

#define PLAYER_FIRE_INTERVAL 10
#define PLAYER_FIRE_RANGE_IN_GRID 8

#define ADD_ZOMBIE_INTERVAL 50
#define PLAYER_RESET_INTERVAL 300

#define ZOMBIE_ROTATE_FRAME_COUNT 8
#define ZOMBIE_HIT_FRONT_FRAME_COUNT 11

#define PLAYER_HIT_FRONT_FRAME_COUNT 11

#define CHECK_INTERFERENCE
// #define DRAW_ORTHOGONAL_FIRE

typedef enum {
    WALK,
    ROTATE,
    ATTACK,
    HIT,
    DIE
} action;

typedef struct {
    int index;
    int x; // Current x position of the zombie
    int y; // Current y position of the zombie
    int blood; // Current HP of the zombie
    int enable; // Whether the zombie is alive
    action action;
    int direction;
    int frame;
} zombie_t;

typedef struct {
    int x; // Current x position of the zombie
    int y; // Current y position of the zombie
    int blood; // Current HP of the zombie
    int direction; // Current direction of the player
    int enable; // Whether the player is alive
    int credit; // Credit for the player
    action action;
    int frame;
    int fire_interval;
    int fire_dist;
    int fire_dist_dx;
    int fire_dist_dy;
} player_t;

#define INDEX_NOTHING -1
#define INDEX_PLAYER_1 254
#define INDEX_PLAYER_2 255

static int grid[GRID_X_COUNT][GRID_Y_COUNT];  // index

static zombie_t zombie[MAX_ZOMBIES_ON_SCREEN];
static player_t player_1, player_2;

static int zombie_appear_counter = 0;
static int cur_zombie_counter = 0;
static int player_1_reset_counter = 0;
static int player_2_reset_counter = 0;

int add_zombie_counter = 0;

/**
 * 7 0 1
 * 6   2
 * 5 4 3
 */

static const int DX[8] = {0, 1, 1, 1, 0, -1, -1, -1};
static const int DY[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
static const int ZOMBIE_HIT_FRAME_MOVEMENT[ZOMBIE_HIT_FRONT_FRAME_COUNT] = {
        8, 6, 4, 4, 3, 3, 2, 2, 1, 1, 1
};
static const int PLAYER_HIT_FRAME_MOVEMENT[PLAYER_HIT_FRONT_FRAME_COUNT] = {
        8, 6, 4, 4, 3, 3, 2, 2, 1, 1, 1
};

int calc_zombie_direction(zombie_t *z) {
    int delta_x, delta_y;
    int delta_x1, delta_y1, delta_x2, delta_y2;
    int closer;

    delta_x1 = TO_GRID(player_1.x) - TO_GRID(z->x);
    delta_y1 = TO_GRID(player_1.y) - TO_GRID(z->y);
    delta_x2 = TO_GRID(player_2.x) - TO_GRID(z->x);
    delta_y2 = TO_GRID(player_2.y) - TO_GRID(z->y);

    if (player_1.enable == 0) {
        closer = 2;
    } else if (player_2.enable == 0) {
        closer = 1;
    } else {
        if (delta_x1 * delta_x1 + delta_y1 * delta_y1 < delta_x2 * delta_x2 + delta_y2 * delta_y2) {
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

    if (delta_x > 0) {
        if (delta_y > 0) return 3;
        else if (delta_y < 0) return 1;
        else return 2;
    } else if (delta_x < 0) {
        if (delta_y > 0) return 5;
        else if (delta_y < 0) return 7;
        else return 6;
    } else {
        if (delta_y > 0) return 4;
        else if (delta_y < 0) return 0;
        else {
            printf("CENTERED DIRECTION!\n");
            return 0;
        }
    }
}

int check_no_interference(int new_x, int new_y, int gx, int gy) {
    int target_x, target_y;
    int id;
    if (gx >= 0 && gx < GRID_X_COUNT && gy >= 0 && gy < GRID_Y_COUNT) {
        id = grid[gx][gy];
        if (id == INDEX_NOTHING) return 0;
        else if (id == INDEX_PLAYER_1) {
            target_x = player_1.x;
            target_y = player_1.y;
        } else if (id == INDEX_PLAYER_2) {
            target_x = player_2.x;
            target_y = player_2.y;
        } else {
            target_x = zombie[id].x;
            target_y = zombie[id].y;
        }
        if ((target_x - new_x) * (target_x - new_x) + (target_y - new_y) * (target_y - new_y) < 1024) return 1;
    } 
    return 0;
}

void try_move_zombie(zombie_t *z, int new_x, int new_y) {
    int dx = DX[z->direction], dy = DY[z->direction];
    int grid_x = TO_GRID(z->x), grid_y = TO_GRID(z->y);

    if (new_x >= MAX_X) {
        new_x = MAX_X - 1;
    } else if (new_x < MIN_X) {
        new_x = MIN_X;
    }

    if (new_y >= MAX_Y) {
        new_y = MAX_Y - 1;
    } else if (new_y < MIN_Y) {
        new_y = MIN_Y;
    }

    int new_grid_x = TO_GRID(new_x), new_grid_y = TO_GRID(new_y);

    if (grid_x == new_grid_x && grid_y == new_grid_y) {
        // Stay in the same grid
    } else if (grid[new_grid_x][new_grid_y] == INDEX_NOTHING) {
        // Move to new grid, no collision
    } else {
        // Collision
        return;
    }

#ifdef CHECK_INTERFERENCE
    switch (z->direction) {
        case 0:
            if (check_no_interference(new_x, new_y, grid_x - 1, grid_y - 1) != 0) return;
            if (check_no_interference(new_x, new_y, grid_x, grid_y - 1) != 0) return;
            if (check_no_interference(new_x, new_y, grid_x + 1, grid_y - 1) != 0) return;
            break;
        case 1:
            if (check_no_interference(new_x, new_y, grid_x, grid_y - 1) != 0) return;
            if (check_no_interference(new_x, new_y, grid_x + 1, grid_y - 1) != 0) return;
            if (check_no_interference(new_x, new_y, grid_x + 1, grid_y) != 0) return;
            break;
        case 2:
            if (check_no_interference(new_x, new_y, grid_x + 1, grid_y - 1) != 0) return;
            if (check_no_interference(new_x, new_y, grid_x + 1, grid_y) != 0) return;
            if (check_no_interference(new_x, new_y, grid_x + 1, grid_y + 1) != 0) return;
            break;
        case 3:
            if (check_no_interference(new_x, new_y, grid_x + 1, grid_y) != 0) return;
            if (check_no_interference(new_x, new_y, grid_x + 1, grid_y + 1) != 0) return;
            if (check_no_interference(new_x, new_y, grid_x, grid_y + 1) != 0) return;
            break;
        case 4:
            if (check_no_interference(new_x, new_y, grid_x + 1, grid_y + 1) != 0) return;
            if (check_no_interference(new_x, new_y, grid_x, grid_y + 1) != 0) return;
            if (check_no_interference(new_x, new_y, grid_x - 1, grid_y + 1) != 0) return;
            break;
        case 5:
            if (check_no_interference(new_x, new_y, grid_x, grid_y + 1) != 0) return;
            if (check_no_interference(new_x, new_y, grid_x - 1, grid_y + 1) != 0) return;
            if (check_no_interference(new_x, new_y, grid_x - 1, grid_y) != 0) return;
            break;
        case 6:
            if (check_no_interference(new_x, new_y, grid_x - 1, grid_y + 1) != 0) return;
            if (check_no_interference(new_x, new_y, grid_x - 1, grid_y) != 0) return;
            if (check_no_interference(new_x, new_y, grid_x - 1, grid_y - 1) != 0) return;
            break;
        case 7:
            if (check_no_interference(new_x, new_y, grid_x - 1, grid_y) != 0) return;
            if (check_no_interference(new_x, new_y, grid_x - 1, grid_y - 1) != 0) return;
            if (check_no_interference(new_x, new_y, grid_x, grid_y - 1) != 0) return;
            break;
        default:
            break;
    }
    
#endif

    z->x = new_x;
    z->y = new_y;

    if (grid[new_grid_x][new_grid_y] == INDEX_NOTHING) {
        // Move to new grid, no collision
        grid[grid_x][grid_y] = INDEX_NOTHING;
        grid[new_grid_x][new_grid_y] = z->index;
    }
}

void update_zombie(zombie_t *z) {
    int dx = DX[z->direction], dy = DY[z->direction];
    int grid_x = TO_GRID(z->x), grid_y = TO_GRID(z->y);
    int face_grid_x = grid_x + dx, face_grid_y = grid_y + dy;

    switch (z->action) {
        case WALK:

            // Initiate attack if player is at the front
            if (face_grid_x >= 0 && face_grid_x < GRID_X_COUNT &&
                face_grid_y >= 0 && face_grid_y < GRID_Y_COUNT &&
                (grid[face_grid_x][face_grid_y] == INDEX_PLAYER_1 || grid[face_grid_x][face_grid_y] == INDEX_PLAYER_2)) {

                z->action = ATTACK;
                z->frame = 0;
                return;
            }

            int new_direction = calc_zombie_direction(z);
            if (new_direction == z->direction) {
                // Do not need to rotate, move
                int new_x = z->x + dx * ZOMBIE_MOVE;
                int new_y = z->y + dy * ZOMBIE_MOVE;
                try_move_zombie(z, new_x, new_y);
            } else {
                // Need to rotate
                z->action = ROTATE;
                z->frame = 0;
                return;
            }

            z->frame++;
            if (z->frame >= ZOMBIE_WALK_FRAME_COUNT) z->frame = 0;

            break;
        case ROTATE:
            z->frame++;
            if (z->frame >= ZOMBIE_ROTATE_FRAME_COUNT) {
                // Change the direction
                z->direction = calc_zombie_direction(z);
                z->action = WALK;
                z->frame = 0;
                return;
            }
            break;
        case ATTACK:
            z->frame++;
            if (z->frame >= ZOMBIE_ATTACK_FRAME_COUNT) {
                // Check whether there is still a player at the facing grid
                if (face_grid_x >= 0 && face_grid_x < GRID_X_COUNT &&
                    face_grid_y >= 0 && face_grid_y < GRID_Y_COUNT &&
                    (grid[face_grid_x][face_grid_y] == INDEX_PLAYER_1 || grid[face_grid_x][face_grid_y] == INDEX_PLAYER_2)) {

                    player_t *target;

                    if (grid[face_grid_x][face_grid_y] == INDEX_PLAYER_1) {
                        target = &player_1;
                    } else {
                        target = &player_2;
                    }

                    if (target->action != DIE) {
                        target->blood -= ZOMBIE_ATTACK;
                        if (target->blood > 0) {
                            target->action = HIT;
                            target->direction = (z->direction + 4) % 8;  // opposite
                        } else {
                            target->action = DIE;
                        }
                        target->frame = 0;
                    }
                }
                z->action = WALK;
                z->frame = 0;
                return;
            }
            break;
        case HIT:
            try_move_zombie(z,
                            z->x - dx * ZOMBIE_HIT_FRAME_MOVEMENT[z->frame],
                            z->y - dy * ZOMBIE_HIT_FRAME_MOVEMENT[z->frame]);
            z->frame++;
            if (z->frame >= ZOMBIE_HIT_FRONT_FRAME_COUNT) {
                if (z->blood > 0) {
                    z->action = WALK;
                    z->frame = 0;
                    return;
                } else {
                    z->action = DIE;
                    z->frame = 0;
                    return;
                }
            }
            break;
        case DIE:

            // z->frame++;
            // if (z->frame >= ZOMBIE_DIE_FRAME_COUNT) {
                z->enable = 0;
                grid[grid_x][grid_y] = INDEX_NOTHING;
                cur_zombie_counter--;
                return;
            // }
            // break;
    }
}

void try_move_player(player_t *p, int new_x, int new_y) {
    int grid_x = TO_GRID(p->x), grid_y = TO_GRID(p->y);

    if (new_x >= MAX_X) {
        new_x = MAX_X - 1;
    } else if (new_x < MIN_X) {
        new_x = MIN_X;
    }

    if (new_y >= MAX_Y) {
        new_y = MAX_Y - 1;
    } else if (new_y < MIN_Y) {
        new_y = MIN_Y;
    }

    int new_grid_x = TO_GRID(new_x), new_grid_y = TO_GRID(new_y);

    if (grid_x == new_grid_x && grid_y == new_grid_y) {
        // Stay in the same grid
    } else if (grid[new_grid_x][new_grid_y] == INDEX_NOTHING) {
        // Move to new grid, no collision
    }  else {
        // Collision, do not move
        return;
    }

#ifdef CHECK_INTERFERENCE
    switch (p->direction) {
        case 0:
            if (check_no_interference(new_x, new_y, grid_x - 1, grid_y - 1) != 0) return;
            if (check_no_interference(new_x, new_y, grid_x, grid_y - 1) != 0) return;
            if (check_no_interference(new_x, new_y, grid_x + 1, grid_y - 1) != 0) return;
            break;
        case 1:
            if (check_no_interference(new_x, new_y, grid_x, grid_y - 1) != 0) return;
            if (check_no_interference(new_x, new_y, grid_x + 1, grid_y - 1) != 0) return;
            if (check_no_interference(new_x, new_y, grid_x + 1, grid_y) != 0) return;
            break;
        case 2:
            if (check_no_interference(new_x, new_y, grid_x + 1, grid_y - 1) != 0) return;
            if (check_no_interference(new_x, new_y, grid_x + 1, grid_y) != 0) return;
            if (check_no_interference(new_x, new_y, grid_x + 1, grid_y + 1) != 0) return;
            break;
        case 3:
            if (check_no_interference(new_x, new_y, grid_x + 1, grid_y) != 0) return;
            if (check_no_interference(new_x, new_y, grid_x + 1, grid_y + 1) != 0) return;
            if (check_no_interference(new_x, new_y, grid_x, grid_y + 1) != 0) return;
            break;
        case 4:
            if (check_no_interference(new_x, new_y, grid_x + 1, grid_y + 1) != 0) return;
            if (check_no_interference(new_x, new_y, grid_x, grid_y + 1) != 0) return;
            if (check_no_interference(new_x, new_y, grid_x - 1, grid_y + 1) != 0) return;
            break;
        case 5:
            if (check_no_interference(new_x, new_y, grid_x, grid_y + 1) != 0) return;
            if (check_no_interference(new_x, new_y, grid_x - 1, grid_y + 1) != 0) return;
            if (check_no_interference(new_x, new_y, grid_x - 1, grid_y) != 0) return;
            break;
        case 6:
            if (check_no_interference(new_x, new_y, grid_x - 1, grid_y + 1) != 0) return;
            if (check_no_interference(new_x, new_y, grid_x - 1, grid_y) != 0) return;
            if (check_no_interference(new_x, new_y, grid_x - 1, grid_y - 1) != 0) return;
            break;
        case 7:
            if (check_no_interference(new_x, new_y, grid_x - 1, grid_y) != 0) return;
            if (check_no_interference(new_x, new_y, grid_x - 1, grid_y - 1) != 0) return;
            if (check_no_interference(new_x, new_y, grid_x, grid_y - 1) != 0) return;
            break;
        default:
            break;
    }
#endif

    p->x = new_x;
    p->y = new_y;

    if (grid[new_grid_x][new_grid_y] == INDEX_NOTHING) {
        grid[grid_x][grid_y] = INDEX_NOTHING;
        grid[new_grid_x][new_grid_y] = (p == &player_1 ? INDEX_PLAYER_1 : INDEX_PLAYER_2);
        
    }
}

void update_player(player_t *p, int direction, int attack) {

    int dx = DX[p->direction], dy = DY[p->direction];
    int grid_x = TO_GRID(p->x), grid_y = TO_GRID(p->y);

    if (p->fire_interval > 0) --p->fire_interval;

    switch (p->action) {
        case WALK:

            if (direction != -1) {
                if (direction == p->direction) {
                    // Do not need to rotate, move
                    int new_x = p->x + dx * PLAYER_MOVE, new_y = p->y + dy * PLAYER_MOVE;
                    try_move_player(p, new_x, new_y);
                } else {
                    // Need to rotate, rotate immediately
                    p->direction = direction;
                    p->action = WALK;
                    p->frame = 0;
                    return;
                }
                p->frame++;
                if (p->frame >= PLAYER_WALK_FRAME_COUNT) p->frame = 0;
            } else {
                p->frame = PLAYER_WALK_FRAME_COUNT - 1;
            }

            if (attack == 1 && p->fire_interval == 0) {
                p->fire_dist_dx = dx;
                p->fire_dist_dy = dy;
                p->fire_dist = PLAYER_FIRE_RANGE_IN_GRID * GRID_SIZE;  // default case

                for (int i = 1; i <= PLAYER_FIRE_RANGE_IN_GRID; ++i) {
                    int gx = grid_x + dx * i;
                    int gy = grid_y + dy * i;
                    if (gx >= 0 && gx < GRID_X_COUNT && gy >= 0 && gy < GRID_Y_COUNT) {
                        int id = grid[gx][gy];
                        if (id != INDEX_NOTHING) {
                            if (id == INDEX_PLAYER_1 || id == INDEX_PLAYER_2) {  
                                // Fiend fire
                                break;  // if not break, it's raygun
                            } else {
                                // Attack zombie
                                if (zombie[id].action != DIE) {
                                    zombie_t *z = &zombie[id];
                                    z->blood -= PLAYER_ATTACK;
                                    if (z->blood > 0) {
                                        z->action = HIT;
                                        z->direction = (p->direction + 4) % 8;  // opposite
                                    } else {
                                        z->action = DIE;
                                    }
                                    z->frame = 0;
                                    p->fire_dist = i * GRID_SIZE;
                                    break;  // if not break, it's raygun
                                }
                            }
                        }
                    }
                }
                p->fire_interval = PLAYER_FIRE_INTERVAL;
            }

            break;
        case HIT:
            try_move_player(p,
                            p->x - dx * PLAYER_HIT_FRAME_MOVEMENT[p->frame],
                            p->y - dy * PLAYER_HIT_FRAME_MOVEMENT[p->frame]);
            p->frame++;
            if (p->frame >= PLAYER_HIT_FRONT_FRAME_COUNT) {
                if (p->blood > 0) {
                    p->action = WALK;
                    p->frame = 0;
                    return;
                } else {
                    p->action = DIE;
                    p->frame = 0;
                    return;
                }
            }
            break;
        case DIE:
            // p->frame++;
            // if (p->frame >= PLAYER_DIE_FRAME_COUNT) {
                p->enable = 0;
                grid[grid_x][grid_y] = INDEX_NOTHING;
                return;
            // }
            // break;
        default:
            printf("INVALID PLAYER STATE!\n");
            break;
    }
}

void reset_player_1() {
    player_1.x = PLAYER_1_BORN_X;
    player_1.y = PLAYER_1_BORN_Y;
    player_1.blood = 100;
    grid[TO_GRID(player_1.x)][TO_GRID(player_1.y)] = INDEX_PLAYER_1;
    player_1.enable = 1;
    player_1.credit = 0;
    player_1.action = WALK;
    player_1.direction = 0;
    player_1.frame = 0;
    player_1.fire_interval = 0;
    player_1.fire_dist = 0;
}

void reset_player_2() {
    player_2.x = PLAYER_2_BORN_X;
    player_2.y = PLAYER_2_BORN_Y;
    player_2.blood = 100;
    grid[TO_GRID(player_2.x)][TO_GRID(player_2.y)] = INDEX_PLAYER_2;
    player_2.enable = 1;
    player_2.credit = 0;
    player_2.action = WALK;
    player_2.direction = 0;
    player_2.frame = 0;
    player_2.fire_interval = 0;
    player_2.fire_dist = 0;
}

void init_game() {
    int i, j;
    for (i = 0; i < GRID_X_COUNT; i++) {
        for (j = 0; j < GRID_Y_COUNT; j++) {
            grid[i][j] = INDEX_NOTHING;
        }
    }

    reset_player_1();
    reset_player_2();
    

    for (i = 0; i < MAX_ZOMBIES_ON_SCREEN; i++) {
        zombie[i].index = i;
        zombie[i].enable = 0;
    }

    add_zombie_counter = 0;
    cur_zombie_counter = 0;
    zombie_appear_counter = 0;
}

int try_add_zombie(zombie_t *z) {
    if (z->enable != 0) return 1;

    // Check whether there is zombie at the born place
    if (grid[TO_GRID(ZOMBIE_BORN_X)][TO_GRID(ZOMBIE_BORN_Y)] != INDEX_NOTHING) {
        return 1;
    } else {
        z->x = ZOMBIE_BORN_X;
        z->y = ZOMBIE_BORN_Y;
        z->enable = 1;
        z->blood = 100;
        z->direction = 4;
        z->action = WALK;
        z->frame = 0;
        grid[TO_GRID(z->x)][TO_GRID(z->y)] = z->index;
        return 0;
    }
}

void draw_zombie(const zombie_t *z) {
    int width, height, flip_x, half_width, half_height;
    switch (z->action) {
        case WALK:
        case ROTATE:
            width = ZOMBIE_WALK_WIDTH[z->direction];
            height = ZOMBIE_WALK_HEIGHT[z->direction];
            flip_x = ZOMBIE_WALK_FLIP_X[z->direction];
            if (z->frame >= ZOMBIE_WALK_FRAME_COUNT) {
                printf("EXCEED ZOMBIE_WALK_FRAME_COUNT!\n");
                return;
            }
            break;
        case ATTACK:
            width = ZOMBIE_ATTACK_WIDTH[z->direction];
            height = ZOMBIE_ATTACK_HEIGHT[z->direction];
            flip_x = ZOMBIE_ATTACK_FLIP_X[z->direction];
            if (z->frame >= ZOMBIE_ATTACK_FRAME_COUNT) {
                printf("EXCEED ZOMBIE_ATTACK_FRAME_COUNT!\n");
                return;
            }
            break;
        case HIT:
            // width = ZOMBIE_HIT_FRONT_WIDTH[z->direction];
            // height = ZOMBIE_HIT_FRONT_HEIGHT[z->direction];
            // flip_x = ZOMBIE_HIT_FRONT_FLIP_X[z->direction];
            width = ZOMBIE_WALK_WIDTH[z->direction];
            height = ZOMBIE_WALK_HEIGHT[z->direction];
            flip_x = ZOMBIE_WALK_FLIP_X[z->direction];
            if (z->frame >= ZOMBIE_HIT_FRONT_FRAME_COUNT) {
                printf("EXCEED ZOMBIE_HIT_FRONT_FRAME_COUNT!\n");
                return;
            }
            break;
        case DIE:
            // width = ZOMBIE_DIE_WIDTH[z->direction];
            // height = ZOMBIE_DIE_HEIGHT[z->direction];
            // flip_x = ZOMBIE_DIE_FLIP_X[z->direction];
            // if (z->frame >= ZOMBIE_DIE_FRAME_COUNT) {
            //     printf("EXCEED ZOMBIE_DIE_FRAME_COUNT!\n");
            //     return;
            // }
            break;
        default:
            printf("INVALID ZOMBIE STATE!\n");
            return;
    }

    half_width = width / 2;
    half_height = height / 2;

    int start_x = z->x - half_width;
    int end_x = z->x + (width - half_width);
    int start_y = z->y - half_height;
    int end_y = z->y + (height - half_height);

    switch (z->action) {
        case WALK:
            draw(start_x, end_x, start_y, end_y, ZOMBIE_WALK_OFFSET[z->direction][z->frame], 0, flip_x);
            break;
        case ROTATE:
            draw(start_x, end_x, start_y, end_y, ZOMBIE_WALK_OFFSET[z->direction][7], 0, flip_x);
            break;
        case ATTACK:
            draw(start_x, end_x, start_y, end_y, ZOMBIE_ATTACK_OFFSET[z->direction][z->frame], 0, flip_x);
            break;
        case HIT:
            // draw(start_x, end_x, start_y, end_y, ZOMBIE_HIT_FRONT_OFFSET[z->direction][z->frame], 0, flip_x);
            draw(start_x, end_x, start_y, end_y, ZOMBIE_WALK_OFFSET[z->direction][0], 0, flip_x);
            break;
        case DIE:
            // draw(start_x, end_x, start_y, end_y, ZOMBIE_DIE_OFFSET[z->direction][z->frame], 0, flip_x);
            break;
        default:
            printf("INVALID ZOMBIE STATE!\n");
            return;
    }
}

void draw_player(const player_t *p) {
    int width, height, flip_x, half_width, half_height;
    switch (p->action) {
        case WALK:
            width = PLAYER_WALK_WIDTH[p->direction];
            height = PLAYER_WALK_HEIGHT[p->direction];
            flip_x = PLAYER_WALK_FLIP_X[p->direction];
            if (p->frame >= PLAYER_WALK_FRAME_COUNT) {
                printf("EXCEED PLAYER_WALK_FRAME_COUNT!\n");
                return;
            }
            break;
        case HIT:
            // width = PLAYER_HIT_FRONT_WIDTH[p->direction];
            // height = PLAYER_HIT_FRONT_HEIGHT[p->direction];
            // flip_x = PLAYER_HIT_FRONT_FLIP_X[p->direction];
            width = PLAYER_WALK_WIDTH[p->direction];
            height = PLAYER_WALK_HEIGHT[p->direction];
            flip_x = PLAYER_WALK_FLIP_X[p->direction];
            if (p->frame >= PLAYER_HIT_FRONT_FRAME_COUNT) {
                printf("EXCEED PLAYER_HIT_FRONT_FRAME_COUNT!\n");
                return;
            }
            break;
        case DIE:
            // width = PLAYER_DIE_WIDTH[p->direction];
            // height = PLAYER_DIE_HEIGHT[p->direction];
            // flip_x = PLAYER_DIE_FLIP_X[p->direction];
            // if (p->frame >= PLAYER_DIE_FRAME_COUNT) {
            //     printf("EXCEED PLAYER_DIE_FRAME_COUNT!\n");
            //     return;
            // }
            break;
        default:
            printf("INVALID PLAYER STATE!\n");
            return;
    }

    half_width = width / 2;
    half_height = height / 2;

    int start_x = p->x - half_width;
    int end_x = p->x + (width - half_width);
    int start_y = p->y - half_height;
    int end_y = p->y + (height - half_height);

    switch (p->action) {
        case WALK:
            draw(start_x, end_x, start_y, end_y, PLAYER_WALK_OFFSET[p->direction][p->frame], 0, flip_x);
            break;
        case HIT:
            // draw(start_x, end_x, start_y, end_y, PLAYER_HIT_FRONT_OFFSET[p->direction][p->frame], 0, flip_x);
            draw(start_x, end_x, start_y, end_y, PLAYER_WALK_OFFSET[p->direction][7], 0, flip_x);
            break;
        case DIE:
            // draw(start_x, end_x, start_y, end_y, PLAYER_DIE_OFFSET[p->direction][p->frame], 0, flip_x);
            break;
        default:
            printf("INVALID PLAYER STATE!\n");
            return;
    }

   if (p->fire_interval > PLAYER_FIRE_INTERVAL / 2) {
       if (p->fire_dist_dx != 0 && p->fire_dist_dy != 0) {
           // Not horizontal or vertical, we can not draw...
#ifdef DRAW_ORTHOGONAL_FIRE
           for (int i = 1; i < p->fire_dist; ++i) {
                start_x = p->x + i * p->fire_dist_dx;
                end_x = start_x + 1;
                start_y = p->y + i * p->fire_dist_dy;
                end_y = start_y + 1;
                draw(start_x, end_x, start_y, end_y, 89138, 0, 0);
           }
#endif
       } else {
           if (p->fire_dist_dx != 0) {
                if (p->fire_dist_dx > 0) {
                    start_y = p->y + 2;
                    end_y = start_y + 1;
                    start_x = p->x + half_width;
                    end_x = p->x + p->fire_dist;
                } else {
                    start_y = p->y - 10;
                    end_y = start_y + 1;
                    end_x = p->x - half_width + 5;
                    start_x = p->x - p->fire_dist;
                }
                for (int x = start_x; x + 13 < end_x; x += 13) {
                    draw(x, x + 13, start_y, end_y, 89138, 0, 0);
                }
           } else {
                if (p->fire_dist_dy > 0) {
                    start_x = p->x - 9;
                    end_x = start_x + 1;
                    start_y = p->y + half_height;
                    end_y = p->y + p->fire_dist;
                } else {
                    start_x = p->x + 11;
                    end_x = start_x + 1;
                    end_y = p->y - half_height;
                    start_y = p->y - p->fire_dist;
                }
                
                for (int y = start_y; y + 13 < end_y; y += 13) {
                    draw(start_x, end_x, y, y + 13, 89138, 0, 0);
                }
           }
       }
   }
}

void draw_blood(const player_t *p, int offset) {
    int start_y, start_x, end_x, x;
    start_y = p->y - 27;
    start_x = p->x - 11;
    end_x = start_x + p->blood / 4;;
    for (x = start_x; x + 4 < end_x; x += 4) {
        draw(x, x + 4, start_y, start_y + 3, offset, 0, 0);
    }
    if (x < end_x) {
        draw(x, end_x, start_y, start_y + 3, offset, 0, 0);
    }
}

int refresh(int should_update, int direction_1, int direction_2, int attack_1, int attack_2) {

    int i, gx, gy, id;

    // for (i = 0; i < MAX_ZOMBIES_ON_SCREEN; ++i) {
    //     if (zombie[i].enable) {
    //         if (should_update) update_zombie(&zombie[i]);
    //         draw_zombie(&zombie[i]);
    //     }
    // }
    // if (player_1.enable) {
    //     if (should_update) update_player(&player_1, direction_1, attack_1);
    // }
    // if (player_1.enable) {
    //     draw_player(&player_1);
    // }
    // if (player_2.enable) {
    //     if (should_update) update_player(&player_2, direction_2, attack_2);
    // }
    // if (player_2.enable) {
    //     draw_player(&player_2);
    // }

    for (gy = 0; gy < GRID_Y_COUNT; ++gy) {
        for (gx = 0; gx < GRID_X_COUNT; ++gx) {
            id = grid[gx][gy];
            if (id != INDEX_NOTHING) {
                if (id == INDEX_PLAYER_1) {
                    if (should_update) {
                        if (player_1.enable) {
                            update_player(&player_1, direction_1, attack_1);
                        }
                    }
                    if (player_1.enable) {
                        draw_player(&player_1);
                    }
                } else if (id == INDEX_PLAYER_2) {
                    if (should_update) {
                        if (player_2.enable) {
                            update_player(&player_2, direction_2, attack_2);
                        }
                    }
                    if (player_2.enable) {
                        draw_player(&player_2);
                    }
                } else {
                    if (zombie[id].enable) {
                        if (should_update) update_zombie(&zombie[id]);
                    }
                    if (zombie[id].enable) {
                        draw_zombie(&zombie[id]);
                    }
                }
            }
        }
    }

    if (should_update) {
        add_zombie_counter++;
        if (add_zombie_counter >= ADD_ZOMBIE_INTERVAL) {
            if ((zombie_appear_counter < MAX_TOTAL_ZOMBIES) && (cur_zombie_counter < MAX_ZOMBIES_ON_SCREEN)) {
                for (i = 0; i < MAX_ZOMBIES_ON_SCREEN; i++) {
                    if (zombie[i].enable == 0) {
                        zombie[i].blood = 100;
                        if (try_add_zombie(&zombie[i]) == 0) {
                            cur_zombie_counter++;
                            zombie_appear_counter++;
                        }
                        break;
                    }
                }
            }
            add_zombie_counter = 0;
        }

        if (!player_1.enable) {
            player_1_reset_counter++;
            if (player_1_reset_counter >= PLAYER_RESET_INTERVAL && grid[TO_GRID(PLAYER_1_BORN_X)][TO_GRID(PLAYER_1_BORN_Y)] == INDEX_NOTHING) {
                reset_player_1();
                player_1_reset_counter = 0;
            }
        }

        if (!player_2.enable) {
            player_2_reset_counter++;
            if (player_2_reset_counter >= PLAYER_RESET_INTERVAL && grid[TO_GRID(PLAYER_2_BORN_X)][TO_GRID(PLAYER_2_BORN_Y)] == INDEX_NOTHING) {
                reset_player_2();
                player_2_reset_counter = 0;
            }
        }

        if ((player_1.enable == 0) && (player_2.enable == 0)) {
            // printf("You are all dead, please restart the game!\n");
            return 0;
        }
        if ((cur_zombie_counter == 0) && (zombie_appear_counter == MAX_TOTAL_ZOMBIES)) {
            // printf("Congratulations, you have killed all the zombies!\n");
            return 0;
        }
    }

    draw_blood(&player_1, 114754);
    draw_blood(&player_2, 122079);


    return should_update;
}
