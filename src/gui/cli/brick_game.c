#include "brick_game.h"

int main(void) {
  initNcurses();
  gameLoop();
  endwin();
  return 0;
}

void gameLoop() {
  initGameBackend(getInitialHighScore());
  bool is_playing = true;
  int action = NoUserAction;
  GameInfo_t gi = {0};
  WINDOW *w_field = getGameFieldWindow();
  WINDOW *w_info = getGameInfoWindow();
  printOverlay(w_info);

  while (is_playing) {
    action = getUserAction();
    if (action == Terminate) is_playing = false;
    if (action != NoUserAction) userInput((UserAction_t)action, false);

    gi = updateCurrentState();
    printCurrentState(&gi, w_field, w_info);
    timeout(GAME_LOOP_DELAY);
  }

  delwin(w_field);
  delwin(w_info);
  saveHighScore(gi.high_score);
  freeGameBackend();
}

int getUserAction() {
  int action = NoUserAction;
  int ch = getch();

  if (ch == KEY_Start) {
    pthread_t music_thread;
  if (pthread_create(&music_thread, NULL, play_music, NULL) != 0) {
    printf("Failed to create stream for music\n");
  }
    action = Start;
  } else if (ch == KEY_Pause)
    action = Pause;
  else if (ch == KEY_Terminate)
    action = Terminate;
  else if (ch == KEY_Left)
    action = Left;
  else if (ch == KEY_Right)
    action = Right;
  else if (ch == KEY_Up)
    action = Up;
  else if (ch == KEY_Down)
    action = Down;
  else if (ch == KEY_Action)
    action = Action;

  return action;
}

int getInitialHighScore() {
  int high_score = 0;
  FILE *file = fopen("high_score.txt", "r");
  if (file != NULL) {
    fscanf(file, "%d", &high_score);
    fclose(file);
  }
  return high_score;
}

void saveHighScore(int high_score) {
  FILE *file = fopen("high_score.txt", "w");
  if (file != NULL) {
    fprintf(file, "%d", high_score);
    fclose(file);
  }
}