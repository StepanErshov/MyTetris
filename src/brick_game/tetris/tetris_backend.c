#include "tetris_backend.h"

int **createMatrix(int rows, int columns) {
  int **result = NULL;

  if (rows > 0 && columns > 0) {
    result = (int **)calloc(rows, sizeof(int *));
    if (result != NULL) {
      result[0] = (int *)calloc(rows * columns, sizeof(int));
      if (result[0] == NULL) {
        free(result);
        result = NULL;
      } else {
        for (int m = 1; m < rows; m++)
          result[m] = (int *)result[0] + m * columns;
      }
    }
  }

  return result;
}

void freeMatrix(int **matrix) {
  if (matrix != NULL) {
    free(matrix[0]);  // free values array
    free(matrix);     // free pointers array
    matrix = NULL;
  }
}

void copyMatrix(int N, int M, int **src, int **dest) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      dest[i][j] = src[i][j];
    }
  }
}

GameParams_t *getStaticGameParams() {
  static GameParams_t tp;
  return &tp;
}

void initGameBackend(int high_score) {
  GameParams_t *gp = getStaticGameParams();
  gp->field = createMatrix(FIELD_N, FIELD_M);
  gp->feild_with_figure = createMatrix(FIELD_N, FIELD_M);
  gp->next = createMatrix(FIGURE_FIELD_N, FIGURE_FIELD_M);
  gp->figure = createMatrix(FIGURE_FIELD_N, FIGURE_FIELD_M);
  gp->check_figure = createMatrix(FIGURE_FIELD_N, FIGURE_FIELD_M);
  gp->high_score = high_score;

  doStart(gp);
  // srand(getMilliSecOfDay());
}

void freeGameBackend() {
  GameParams_t *gp = getStaticGameParams();
  freeMatrix(gp->field);
  freeMatrix(gp->next);
  freeMatrix(gp->figure);
  freeMatrix(gp->check_figure);
  freeMatrix(gp->feild_with_figure);
}

long long getMilliSecOfDay() {
  struct timeval tval;
  gettimeofday(&tval, NULL);
  return (long long)tval.tv_sec * 1000 + tval.tv_usec / 1000;
}

long long getShiftingInterval(GameParams_t *gp) {
  return (long long)(60 * 1000) /
         (SHIFTS_PER_MIN_LVL1 + SHIFTS_PER_MIN_LVL_INCR * (gp->level - 1));
}

void clearField(int N, int M, int **field) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      field[i][j] = PIXEL_EMPTY;
    }
  }
}

FigureType_t getRandomFigureType() {
  FigureType_t f_type = getMilliSecOfDay() % FIGURE_TYPE_COUNT;
  // srand(getMilliSecOfDay()); // the seed to be set in initGameBackend()
  // FigureType_t f_type = rand() % FIGURE_TYPE_COUNT; // not work in WSL?
  return f_type;
}

void setFigurePixels(FigureType_t f_type, FigureOrientation_t f_orient,
                     int pixel_val, int **f_field, bool clear_f_field) {
  // f_type = 4 % FIGURE_TYPE_COUNT; // DEBUG
  // f_orient = 0 % FIGURE_ORIENTATIONS_COUNT; // DEBUG
  /* массив фигур [тип фигуры][ориентация фигуры][массив координат заполненных
   * ячеек фигуры] */
  const PPos_t figure_maps[FIGURE_TYPE_COUNT][FIGURE_ORIENTATIONS_COUNT]
                          [FIGURE_FIELD_SIZE] = {
                              // I
                              {{{0, 0}, {0, 1}, {0, 2}, {0, 3}},
                               {{0, 1}, {1, 1}, {2, 1}, {3, 1}},
                               {{0, 0}, {0, 1}, {0, 2}, {0, 3}},
                               {{0, 1}, {1, 1}, {2, 1}, {3, 1}}},
                              // J
                              {{{0, 0}, {1, 0}, {1, 1}, {1, 2}},
                               {{0, 1}, {0, 2}, {1, 1}, {2, 1}},
                               {{1, 0}, {1, 1}, {1, 2}, {2, 2}},
                               {{0, 1}, {1, 1}, {2, 0}, {2, 1}}},
                              // L
                              {{{0, 2}, {1, 0}, {1, 1}, {1, 2}},
                               {{0, 1}, {1, 1}, {2, 1}, {2, 2}},
                               {{1, 0}, {1, 1}, {1, 2}, {2, 0}},
                               {{0, 0}, {0, 1}, {1, 1}, {2, 1}}},
                              // O
                              {{{0, 1}, {0, 2}, {1, 1}, {1, 2}},
                               {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
                               {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
                               {{0, 1}, {0, 2}, {1, 1}, {1, 2}}},
                              // S
                              {{{0, 1}, {0, 2}, {1, 0}, {1, 1}},
                               {{0, 1}, {1, 1}, {1, 2}, {2, 2}},
                               {{1, 1}, {1, 2}, {2, 0}, {2, 1}},
                               {{0, 0}, {1, 0}, {1, 1}, {2, 1}}},
                              // T
                              {{{0, 1}, {1, 0}, {1, 1}, {1, 2}},
                               {{0, 1}, {1, 1}, {1, 2}, {2, 1}},
                               {{1, 0}, {1, 1}, {1, 2}, {2, 1}},
                               {{0, 1}, {1, 0}, {1, 1}, {2, 1}}},
                              // Z
                              {{{0, 0}, {0, 1}, {1, 1}, {1, 2}},
                               {{0, 2}, {1, 1}, {1, 2}, {2, 1}},
                               {{1, 0}, {1, 1}, {2, 1}, {2, 2}},
                               {{0, 1}, {1, 0}, {1, 1}, {2, 0}}},
                          };
  if (clear_f_field == true)
    clearField(FIGURE_FIELD_SIZE, FIGURE_FIELD_SIZE, f_field);
  for (int i = 0; i < FIGURE_FIELD_SIZE; i++) {
    int f_map_x = figure_maps[f_type][f_orient][i].x;
    int f_map_y = figure_maps[f_type][f_orient][i].y;
    f_field[f_map_y][f_map_x] = pixel_val;
  }
}

void addFigurePixelsOnField(GameParams_t *gp, int **field) {
  for (int i = 0; i < FIGURE_FIELD_N; i++) {
    for (int j = 0; j < FIGURE_FIELD_M; j++) {
      if (gp->figure[i][j] == PIXEL_FILLED)
        field[i + gp->f_pos.y][j + gp->f_pos.x] = PIXEL_FILLED;
    }
  }
}

bool checkFigureOnField(GameParams_t *gp, PPos_t f_pos,
                        FigureOrientation_t f_orient) {
  bool check_res = true;

  setFigurePixels(gp->f_type, f_orient, PIXEL_FILLED, gp->check_figure, true);

  for (int i = 0, y = f_pos.y; i < FIGURE_FIELD_N && check_res == true;
       i++, y++) {
    for (int j = 0, x = f_pos.x; j < FIGURE_FIELD_M && check_res == true;
         j++, x++) {
      if (gp->check_figure[i][j] == PIXEL_FILLED) {
        if (y < 0 || y >= FIELD_N || x < 0 || x >= FIELD_M ||
            gp->field[y][x] == PIXEL_FILLED) {
          check_res = false;
        }
      }
    }
  }
  return check_res;
}

bool checkNextFigureOnField(GameParams_t *gp) {
  bool check_res = true;

  for (int i = 0, y = START_FIGURE_Y; i < FIGURE_FIELD_N && check_res == true;
       i++, y++) {
    for (int j = 0, x = START_FIGURE_X; j < FIGURE_FIELD_M && check_res == true;
         j++, x++) {
      if (gp->next[i][j] == PIXEL_FILLED) {
        if (y < 0 || y >= FIELD_N || x < 0 || x >= FIELD_M ||
            gp->field[y][x] == PIXEL_FILLED) {
          check_res = false;
        }
      }
    }
  }
  return check_res;
}

bool checkFigureAttaching(GameParams_t *gp) {
  bool check_res = false;

  for (int i = 0, y = gp->f_pos.y; i < FIGURE_FIELD_N && check_res == false;
       i++, y++) {
    for (int j = 0, x = gp->f_pos.x; j < FIGURE_FIELD_M && check_res == false;
         j++, x++) {
      if (gp->check_figure[i][j] == PIXEL_FILLED) {
        if (y + 1 == FIELD_N || gp->field[y + 1][x] == PIXEL_FILLED) {
          check_res = true;
        }
      }
    }
  }
  return check_res;
}

void updateScoreLevelSpeed(GameParams_t *gp, int filled_lines_count) {
  if (filled_lines_count == 1) {
    gp->score = gp->score + POINTS_1_LINE;
  } else if (filled_lines_count == 2) {
    gp->score = gp->score + POINTS_2_LINE;
  } else if (filled_lines_count == 3) {
    gp->score = gp->score + POINTS_3_LINE;
  } else if (filled_lines_count == 4) {
    gp->score = gp->score + POINTS_4_LINE;
  }

  if (gp->score > gp->high_score) gp->high_score = gp->score;
  gp->level = (LVL_MIN + gp->score / POINTS_PER_LVL) > LVL_MAX
                  ? LVL_MAX
                  : (LVL_MIN + gp->score / POINTS_PER_LVL);
  gp->speed = gp->level;
}

// return number of removed full-filled lines (0 - no lines removed)
int removeFullFilledLines(GameParams_t *gp) {
  int filled_lines_count = 0;
  int line_y = FIELD_N - 1;
  while (line_y >= 0) {
    if (checkLineIsFull(gp->field, line_y) == true) {
      filled_lines_count++;
      moveDownLinesAbove(gp->field, line_y);
    } else {
      line_y--;
    }
  }
  return filled_lines_count;
}

bool checkLineIsFull(int **field, int line_y) {
  bool check_res = true;
  for (int j = 0; j < FIELD_M && check_res == true; j++)
    if (field[line_y][j] != PIXEL_FILLED) check_res = false;

  return check_res;
  ;
}

void moveDownLinesAbove(int **field, int line_y) {
  for (int i = line_y; i >= 0; i--) {
    for (int j = 0; j < FIELD_M; j++) {
      if (i > 0) {
        field[i][j] = field[i - 1][j];
      } else if (i == 0) {
        field[i][j] = PIXEL_EMPTY;
      }
    }
  }
}
