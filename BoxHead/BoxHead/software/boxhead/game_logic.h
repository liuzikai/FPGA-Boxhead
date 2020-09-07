/**
 * Header file for the game logic for ECE 385 final project
 */

#ifndef _GAME_LOGIC_H_
#define _GAME_LOGIC_H_

// Constants

#define WINDOW_WIDTH 40 // discrete positions in the width of the screen
#define WINDOW_HEIGHT 20 // discrete positions in the width of the screen
#define MAX_TOTAL_ZOMBIES 10 // maximum number of zombies a player need to kill in one game 
#define MAX_ZOMBIES_ON_SCREEN 5 // maximum number of zombies that can appear on the screen
#define ZOMBIE_MOVE 1 // moving speed for zombies
#define PLAYER_MOVE 1 // moving speed for player
// #define STEP_LENGTH 2 // step length for both the player and zombies
#define ZOMBIE_ATTACT 10 // HP decrease when a zombie is close to the player
#define PLAYER_ATTACT 50 // HP decrease when the player shoots at a zombie
#define LASER_RANGE 2 // The laser gun attack range is (LASER_RANGE * LASER_RANGE - 1)

extern int draw_one_frame(int direction_1, int direction_2, int attack_1, int attack_2);

#endif