#ifndef BRICK_GAME_H
#define BRICK_GAME_H

#include <stdio.h>


#include "../../brick_game/tetris/tetris_backend.h"
#include "brick_game_frontend.h"

#define GAME_LOOP_DELAY 10  // ms

#define KEY_Start 's'
#define KEY_Pause 'p'
#define KEY_Terminate 'q'
#define KEY_Left KEY_LEFT
#define KEY_Right KEY_RIGHT
#define KEY_Up KEY_UP  // not used in tetris
#define KEY_Down KEY_DOWN
#define KEY_Action 'z'   // 'Rotate' in tetris
#define NoUserAction -1  // getch() return ERR (-1) in case of no input

void gameLoop();
int getUserAction();
int getInitialHighScore();
void saveHighScore(int high_score);


#endif