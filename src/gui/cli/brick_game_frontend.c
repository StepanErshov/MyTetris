#include "brick_game_frontend.h"

void initNcurses() {
  initscr();
  cbreak();  // disable the buffering, get a character-at-a-time input
  noecho();
  keypad(stdscr, TRUE);   // to capture special keys by getch(), e.g. arrow keys
  nodelay(stdscr, TRUE);  // getch() will work in a non-blocking manner
                          // it will return ERR if the key input is not ready.
  curs_set(0);  // sets cursor state to invisible/normal/very visible (0/1/2)
  scrollok(stdscr, TRUE);  // "window scrolling is ON"

  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLACK);  // for empty field pixels
  init_pair(2, COLOR_CYAN, COLOR_CYAN);    // for tetris bricks pixels
  refresh();
}

WINDOW *getGameFieldWindow() {
  WINDOW *w_field =
      newwin(FIELD_ROWS, FIELD_COLS, MARGIN_ROWS * 2, MARGIN_COLS * 2);
  return w_field;
}

WINDOW *getGameInfoWindow() {
  WINDOW *w_info = newwin(INFO_ROWS, INFO_COLS, MARGIN_ROWS * 2,
                          (FIELD_COLS - 1) + MARGIN_COLS * (1 + 2 + 2 + 1));
  return w_info;
}

void printCurrentState(GameInfo_t *gi, WINDOW *w_field, WINDOW *w_info) {
  printField(FIELD_N, FIELD_M, gi->field, w_field);
  printGameInfo(gi, w_info);
}

void printOverlay(WINDOW *w_info) {
  WINDOW *w_field_overlay =
      newwin(FIELD_ROWS + MARGIN_ROWS * 2, FIELD_COLS + MARGIN_COLS * 2,
             MARGIN_ROWS, MARGIN_COLS);
  box(w_field_overlay, 0, 0);
  mvwprintw(w_field_overlay, 0, (w_field_overlay->_maxx - 5) / 2, "Tetris");
  wrefresh(w_field_overlay);
  delwin(w_field_overlay);

  WINDOW *w_info_overlay = newwin(
      INFO_ROWS + MARGIN_ROWS * 2, INFO_COLS + MARGIN_COLS * 2, MARGIN_ROWS,
      MARGIN_COLS + w_field_overlay->_maxy + MARGIN_COLS * 2);
  box(w_info_overlay, 0, 0);
  mvwprintw(w_info_overlay, 0, (w_info_overlay->_maxx - 9) / 2, "Game_Info");
  wrefresh(w_info_overlay);
  delwin(w_info_overlay);

  mvwprintw(w_info, 0, 0, "Next:");
  mvwprintw(w_info, 13, 0, "Controls:");
  mvwprintw(w_info, 14, 0, "  Left/Right -> Move");
  mvwprintw(w_info, 15, 0, "  Down       -> Drop");
  mvwprintw(w_info, 16, 0, "  z          -> Rotate");
  mvwprintw(w_info, 17, 0, "  s  -> Start/Repeat");
  mvwprintw(w_info, 18, 0, "  p  -> Pause/Continue");
  mvwprintw(w_info, 19, 0, "  q  -> Quit");

  wrefresh(w_info);
}

void printGameInfo(GameInfo_t *gi, WINDOW *w_info) {
  WINDOW *w_next = newwin(FIGURE_FIELD_ROWS, FIGURE_FIELD_COLS,
                          w_info->_begy + 2, w_info->_begx + 2);
  printField(FIGURE_FIELD_N, FIGURE_FIELD_M, gi->next, w_next);
  // wrefresh(w_next);
  delwin(w_next);

  mvwprintw(w_info, 7, 0, "High Score: %d      ", gi->high_score);
  mvwprintw(w_info, 8, 0, "Score: %d      ", gi->score);
  mvwprintw(w_info, 9, 0, "Level: %d      ", gi->level);
  mvwprintw(w_info, 10, 0, "Speed: %d      ", gi->speed);
  mvwprintw(w_info, 11, 0, "Game Status: %s", getGameStatusStr(gi));

  wrefresh(w_info);
}

char *getGameStatusStr(GameInfo_t *gi) {
  char *str = NULL;

  if (gi->pause == StartStatePause)
    str = "READY TO START";
  else if (gi->pause == PauseStatePause)
    str = "PAUSE         ";
  else if (gi->pause == GameOverStatePause)
    str = "GAME OVER     ";
  else  // NoPause
    str = "PLAY          ";
  return str;
}

void printField(int N, int M, int **field, WINDOW *w_field) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      if (field[i][j] != PIXEL_EMPTY) {
        wattron(w_field, COLOR_PAIR(2));
        mvwprintw(w_field, i, j * COLS_PER_PIXEL, "  ");
        wattroff(w_field, COLOR_PAIR(1));
      } else {
        wattron(w_field, COLOR_PAIR(1));
        mvwprintw(w_field, i, j * COLS_PER_PIXEL, "  ");
        wattroff(w_field, COLOR_PAIR(1));
      }
    }
  }
  wrefresh(w_field);
}
