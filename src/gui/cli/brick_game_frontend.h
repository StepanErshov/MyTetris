#ifndef BRICK_GAME_FRONTEND_H
#define BRICK_GAME_FRONTEND_H

#include <ncurses.h>
#include <stdlib.h> 
#include <time.h>  

#include "../../brick_game/tetris/tetris_backend.h"

#define ROWS_PER_PIXEL 1
#define COLS_PER_PIXEL 2

#define FIELD_ROWS (FIELD_N * ROWS_PER_PIXEL)
#define FIELD_COLS (FIELD_M * COLS_PER_PIXEL)

#define FIGURE_FIELD_ROWS (FIGURE_FIELD_N * ROWS_PER_PIXEL)
#define FIGURE_FIELD_COLS (FIGURE_FIELD_M * COLS_PER_PIXEL)

#define INFO_ROWS FIELD_ROWS
#define INFO_COLS 27

#define MARGIN_ROWS 1
#define MARGIN_COLS 1

// typedef enum GameStatus {
//   ReadyToStart,
//   Playing,
//   PauseGame,
//   GameOver
// } GameStatus_t;

void initNcurses();
WINDOW *getGameFieldWindow();
WINDOW *getGameInfoWindow();
void printCurrentState(GameInfo_t *gi, WINDOW *w_field, WINDOW *w_info);
void printOverlay(WINDOW *w_info);
void printGameInfo(GameInfo_t *gi, WINDOW *w_info);
void printField(int N, int M, int **field, WINDOW *w_field);
char *getGameStatusStr(GameInfo_t *gi);
int getRandomColor();

#endif
