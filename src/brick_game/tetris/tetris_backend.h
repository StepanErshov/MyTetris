#ifndef TETRIS_BACKEND_H
#define TETRIS_BACKEND_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/time.h>

#define FIELD_N 20
#define FIELD_M 10
#define PIXEL_EMPTY 0
#define PIXEL_FILLED 1

#define FIGURE_FIELD_SIZE 4
#define FIGURE_FIELD_N FIGURE_FIELD_SIZE
#define FIGURE_FIELD_M FIGURE_FIELD_SIZE
#define FIGURE_TYPE_COUNT 7
#define FIGURE_ORIENTATIONS_COUNT 4

#define LVL_MIN 1
#define LVL_MAX 10
#define SHIFTS_PER_MIN_LVL1 60
#define SHIFTS_PER_MIN_LVL_INCR 12

#define POINTS_1_LINE 100
#define POINTS_2_LINE 300
#define POINTS_3_LINE 700
#define POINTS_4_LINE 1500
#define POINTS_PER_LVL 600

#define START_FIGURE_X 3
#define START_FIGURE_Y 0
#define MOVING_START_TIME_NOT_SET -1

// requirements for brick game API (including Tetris) from the S21 project task
typedef enum {
  Start,  // = 0
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

// void userInput(UserAction_t action, bool hold);
// GameInfo_t updateCurrentState();
// ===========================================================================

typedef enum PauseValue {
  NoPause = 0,        // Playing
  StartStatePause,    // Waiting to start
  PauseStatePause,    // Waiting to continue game
  GameOverStatePause  // Waiting to Quit or Repeat game
} PauseValue_t;

typedef enum GameState {
  START = 0,
  SPAWN,
  MOVING,
  SHIFTING,
  PAUSE,
  ATTACHING,
  GAMEOVER
} GameState_t;

typedef enum FigureType {
  FTYPE_I,
  FTYPE_J,
  FTYPE_L,
  FTYPE_O,
  FTYPE_S,
  FTYPE_T,
  FTYPE_Z
} FigureType_t;

typedef enum FigureOrientation {
  F_ORIENT_0,
  F_ORIENT_1,
  F_ORIENT_2,
  F_ORIENT_3
} FigureOrientation_t;

typedef struct PixelPosition {
  int y;
  int x;
} PPos_t;

typedef struct GameParams {
  int **field;
  int **feild_with_figure;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  PauseValue_t pause;
  GameState_t state;

  int **figure;
  FigureType_t f_type;
  FigureOrientation_t f_orient;
  PPos_t f_pos;

  int **check_figure;

  FigureType_t next_type;

  long long MOVING_start_time;
  // long long MOVING_PAUSE_start_time;
} GameParams_t;

typedef void (*fsmActionFunc)(GameParams_t *gp);

void doStart(GameParams_t *gp);
void doSpawn(GameParams_t *gp);
void doLeft(GameParams_t *gp);
void doRight(GameParams_t *gp);
void doRotate(GameParams_t *gp);
void doDown(GameParams_t *gp);
void doShifting(GameParams_t *gp);
void doPause(GameParams_t *gp);
void doMoving(GameParams_t *gp);
void doAttaching(GameParams_t *gp);
void doGameOver(GameParams_t *gp);
void *play_music();

void userInput(UserAction_t action, bool hold);
GameInfo_t updateCurrentState();

int **createMatrix(int rows, int columns);
void freeMatrix(int **matrix);
void copyMatrix(int N, int M, int **src, int **dest);
GameParams_t *getStaticGameParams();
void initGameBackend(int high_score);
void freeGameBackend();
long long getMilliSecOfDay();
void fsmInternalTimeRelatedAction(GameParams_t *gp);
long long getShiftingInterval(GameParams_t *gp);
void clearField(int N, int M, int **field);
FigureType_t getRandomFigureType();
void setFigurePixels(FigureType_t f_type, FigureOrientation_t f_orient,
                     int pixel_val, int **f_field, bool clear_f_field);
void addFigurePixelsOnField(GameParams_t *gp, int **field);

bool checkFigureOnField(GameParams_t *gp, PPos_t f_pos,
                        FigureOrientation_t f_orient);
bool checkNextFigureOnField(GameParams_t *gp);
bool checkFigureAttaching(GameParams_t *gp);
void updateScoreLevelSpeed(GameParams_t *gp, int filled_lines_count);
int removeFullFilledLines(GameParams_t *gp);
bool checkLineIsFull(int **field, int line_y);
void moveDownLinesAbove(int **field, int line_y);

#endif
