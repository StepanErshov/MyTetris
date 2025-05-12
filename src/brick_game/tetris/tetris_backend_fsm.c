#include "tetris_backend.h"

// This is a finite state machine realisation based on matrix of "actions".
/*
    Function userInput() takes an action function from fsm_table.
    GameState_t state defines an index of row in matrix (where to get).
    UserAction defines an index of column in matrix (what to get).
*/
void userInput(UserAction_t action, bool hold) {
  hold = hold;  // param hold is not used in Tetris game

  // GameStates (7): START, SPAWN, MOVING, SHIFTING, PAUSE, ATTACHING, GAMEOVER
  // UserActions (8): Start, Pause, Terminate, Left, Right, Up, Down, Action
  const fsmActionFunc fsm_table[7][8] = {
      {doSpawn, NULL, NULL, NULL, NULL, NULL, NULL, NULL},             // START
      {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},                // SPAWN
      {NULL, doPause, NULL, doLeft, doRight, NULL, doDown, doRotate},  // MOVING
      {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},      // SHIFTING
      {NULL, doMoving, NULL, NULL, NULL, NULL, NULL, NULL},  // PAUSE
      {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},      // ATTACHING
      {doStart, NULL, NULL, NULL, NULL, NULL, NULL, NULL}    // GAMEOVER
  };

  fsmActionFunc action_func = NULL;
  GameParams_t *gp = getStaticGameParams();
  action_func = fsm_table[gp->state][action];
  if (action_func != NULL) action_func(gp);
}

GameInfo_t updateCurrentState() {
  GameParams_t *gp = getStaticGameParams();
  fsmInternalTimeRelatedAction(gp);

  GameInfo_t gi = {0};

  copyMatrix(FIELD_N, FIELD_M, gp->field, gp->feild_with_figure);
  addFigurePixelsOnField(gp, gp->feild_with_figure);
  gi.field = gp->feild_with_figure;

  gi.next = gp->next;
  gi.score = gp->score;
  gi.high_score = gp->high_score;
  gi.level = gp->level;
  gi.speed = gp->speed;
  gi.pause = (int)gp->pause;

  return gi;
}

void fsmInternalTimeRelatedAction(GameParams_t *gp) {
  if (gp->state == MOVING) {
    long long curr_time = getMilliSecOfDay();
    if (curr_time - gp->MOVING_start_time >= getShiftingInterval(gp) ||
        curr_time - gp->MOVING_start_time < 0  // for "next day" case
    ) {
      doShifting(gp);
    }
  }
}

void doStart(GameParams_t *gp) {
  clearField(FIGURE_FIELD_N, FIGURE_FIELD_M, gp->figure);
  gp->f_pos.y = START_FIGURE_Y;
  gp->f_pos.x = START_FIGURE_X;
  gp->f_orient = F_ORIENT_0;
  gp->f_type = FTYPE_I;
  copyMatrix(FIGURE_FIELD_N, FIGURE_FIELD_M, gp->figure, gp->check_figure);
  clearField(FIELD_N, FIELD_M, gp->field);

  gp->next_type = getRandomFigureType();
  setFigurePixels(gp->next_type, F_ORIENT_0, PIXEL_FILLED, gp->next, true);
  gp->score = 0;
  gp->level = 1;
  gp->speed = 1;

  gp->state = START;
  gp->pause = StartStatePause;
}

void doSpawn(GameParams_t *gp) {
  gp->state = SPAWN;

  gp->f_pos.y = START_FIGURE_Y;
  gp->f_pos.x = START_FIGURE_X;
  gp->f_orient = F_ORIENT_0;
  gp->f_type = gp->next_type;
  copyMatrix(FIGURE_FIELD_N, FIGURE_FIELD_M, gp->next, gp->figure);

  gp->next_type = getRandomFigureType();
  setFigurePixels(gp->next_type, F_ORIENT_0, PIXEL_FILLED, gp->next, true);

  doMoving(gp);
}

void doLeft(GameParams_t *gp) {
  // gp->state = MOVING;
  PPos_t new_f_pos = {0};
  new_f_pos.y = gp->f_pos.y;
  new_f_pos.x = gp->f_pos.x - 1;
  bool check_pos = checkFigureOnField(gp, new_f_pos, gp->f_orient);
  if (check_pos == true) gp->f_pos.x = new_f_pos.x;
}

void doRight(GameParams_t *gp) {
  // gp->state = MOVING;
  PPos_t new_f_pos = {0};
  new_f_pos.y = gp->f_pos.y;
  new_f_pos.x = gp->f_pos.x + 1;
  bool check_pos = checkFigureOnField(gp, new_f_pos, gp->f_orient);
  if (check_pos == true) gp->f_pos.x = new_f_pos.x;
}

void doRotate(GameParams_t *gp) {
  // gp->state = MOVING;
  FigureOrientation_t new_f_orient =
      (gp->f_orient + 1) % FIGURE_ORIENTATIONS_COUNT;
  bool check_pos = checkFigureOnField(gp, gp->f_pos, new_f_orient);
  if (check_pos == true) {
    gp->f_orient = new_f_orient;
    setFigurePixels(gp->f_type, gp->f_orient, PIXEL_FILLED, gp->figure, true);
  }
  // TODO Optional
  //  if check_pos = false -> try to shift figure left-right to get "true"
}

void doDown(GameParams_t *gp) {
  // gp->state = MOVING;
  PPos_t new_f_pos = {0};
  new_f_pos.y = gp->f_pos.y + 1;
  new_f_pos.x = gp->f_pos.x;
  while (checkFigureOnField(gp, new_f_pos, gp->f_orient) == true) {
    gp->f_pos.y = new_f_pos.y;
    new_f_pos.y++;
  }
  doAttaching(gp);
}

void doShifting(GameParams_t *gp) {
  gp->state = SHIFTING;

  PPos_t new_f_pos = {0};
  new_f_pos.y = gp->f_pos.y + 1;
  new_f_pos.x = gp->f_pos.x;
  bool check_pos = checkFigureOnField(gp, new_f_pos, gp->f_orient);
  if (check_pos == true) gp->f_pos.y = new_f_pos.y;

  if (checkFigureAttaching(gp) == false)
    doMoving(gp);
  else
    doAttaching(gp);
}

void doPause(GameParams_t *gp) {
  gp->state = PAUSE;
  gp->pause = PauseStatePause;
  // TODO Optional
  // add and use gp->MOVING_PAUSE_start_time = getMilliSecOfDay();
  // to correct (reduce) rest of time before "doShifting"
}

void doMoving(GameParams_t *gp) {
  gp->state = MOVING;
  gp->pause = NoPause;
  gp->MOVING_start_time = getMilliSecOfDay();
}

void doAttaching(GameParams_t *gp) {
  gp->state = ATTACHING;

  // attach/add figure filled pixels to the field
  for (int i = 0, y = gp->f_pos.y; i < FIGURE_FIELD_N; i++, y++)
    for (int j = 0, x = gp->f_pos.x; j < FIGURE_FIELD_M; j++, x++)
      if (gp->check_figure[i][j] == PIXEL_FILLED)
        gp->field[y][x] = PIXEL_FILLED;

  // remove full filled lines if any
  int filled_lines_count = removeFullFilledLines(gp);

  // update score & high_score, level and speed
  updateScoreLevelSpeed(gp, filled_lines_count);

  // check that next figure can be placed in start position
  if (checkNextFigureOnField(gp) == true)
    doSpawn(gp);
  else
    doGameOver(gp);
}

void doGameOver(GameParams_t *gp) {
  gp->state = GAMEOVER;
  gp->pause = GameOverStatePause;
}
