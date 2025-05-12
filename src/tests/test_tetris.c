#include "brick_game_tests.h"

START_TEST(init_1) {
  int high_score = 12345;
  initGameBackend(high_score);

  GameParams_t *gp = getStaticGameParams();
  GameInfo_t gi = updateCurrentState();

  // GameParams_t *gp asserts
  ck_assert_ptr_nonnull(gp->field);
  ck_assert_ptr_nonnull(gp->feild_with_figure);
  ck_assert_ptr_nonnull(gp->next);
  ck_assert_ptr_nonnull(gp->figure);
  ck_assert_ptr_nonnull(gp->check_figure);
  ck_assert_int_eq(gp->high_score, 12345);
  // clearField(FIGURE_FIELD_N, FIGURE_FIELD_M, gp->figure);
  // clearField(FIELD_N, FIELD_M, gp->field);
  // gp->next_type = getRandomFigureType();
  // setFigurePixels(gp->next_type, F_ORIENT_0, PIXEL_FILLED, gp->next, true);
  ck_assert_int_eq(gp->f_orient, F_ORIENT_0);
  ck_assert_int_eq(gp->score, 0);
  ck_assert_int_eq(gp->level, 1);
  ck_assert_int_eq(gp->speed, 1);
  ck_assert_int_eq(gp->state, START);
  ck_assert_int_eq(gp->pause, StartStatePause);

  // GameInfo_t gi asserts
  // copyMatrix(FIELD_N, FIELD_M, gp->field, gp->feild_with_figure);
  // addFigurePixelsOnField(gp, gp->feild_with_figure);
  ck_assert_ptr_eq(gi.field, gp->feild_with_figure);
  ck_assert_ptr_eq(gi.next, gp->next);
  ck_assert_int_eq(gi.score, gp->score);
  ck_assert_int_eq(gi.high_score, gp->high_score);
  ck_assert_int_eq(gi.level, gp->level);
  ck_assert_int_eq(gi.speed, gp->speed);
  ck_assert_int_eq(gi.pause, (int)gp->pause);

  freeGameBackend();
}
END_TEST

START_TEST(action_Start_1) {
  int high_score = 12345;
  initGameBackend(high_score);

  userInput(Start, false);
  GameParams_t *gp = getStaticGameParams();
  GameInfo_t gi = updateCurrentState();

  ck_assert_int_eq(gp->pause, NoPause);
  ck_assert_int_eq(gp->state, MOVING);
  ck_assert_int_eq(gp->f_pos.y, START_FIGURE_Y);
  ck_assert_int_eq(gp->f_pos.x, START_FIGURE_X);

  ck_assert_int_eq(gi.pause, NoPause);

  freeGameBackend();
}
END_TEST

START_TEST(action_Pause_1) {
  int high_score = 12345;
  initGameBackend(high_score);

  userInput(Start, false);
  userInput(Pause, false);
  GameParams_t *gp = getStaticGameParams();
  GameInfo_t gi = updateCurrentState();

  ck_assert_int_eq(gp->pause, PauseStatePause);
  ck_assert_int_eq(gp->state, PAUSE);
  ck_assert_int_eq(gp->f_pos.y, START_FIGURE_Y);
  ck_assert_int_eq(gp->f_pos.x, START_FIGURE_X);

  ck_assert_int_eq(gi.pause, PauseStatePause);

  freeGameBackend();
}
END_TEST

START_TEST(action_Left_1) {
  int high_score = 12345;
  initGameBackend(high_score);

  userInput(Start, false);
  userInput(Left, false);
  GameParams_t *gp = getStaticGameParams();
  GameInfo_t gi = updateCurrentState();

  ck_assert_int_eq(gp->pause, NoPause);
  ck_assert_int_eq(gp->state, MOVING);
  ck_assert_int_eq(gp->f_pos.y, START_FIGURE_Y);
  ck_assert_int_eq(gp->f_pos.x, START_FIGURE_X - 1);

  ck_assert_int_eq(gi.pause, NoPause);

  freeGameBackend();
}
END_TEST

START_TEST(action_Right_1) {
  int high_score = 12345;
  initGameBackend(high_score);

  userInput(Start, false);
  userInput(Right, false);
  GameParams_t *gp = getStaticGameParams();
  GameInfo_t gi = updateCurrentState();

  ck_assert_int_eq(gp->pause, NoPause);
  ck_assert_int_eq(gp->state, MOVING);
  ck_assert_int_eq(gp->f_pos.y, START_FIGURE_Y);
  ck_assert_int_eq(gp->f_pos.x, START_FIGURE_X + 1);

  ck_assert_int_eq(gi.pause, NoPause);

  freeGameBackend();
}
END_TEST

START_TEST(action_Down_1) {
  int high_score = 12345;
  initGameBackend(high_score);

  userInput(Start, false);
  userInput(Left, false);
  userInput(Left, false);
  userInput(Down, false);
  GameParams_t *gp = getStaticGameParams();
  GameInfo_t gi = updateCurrentState();

  ck_assert_int_eq(gp->pause, NoPause);
  ck_assert_int_eq(gp->state, MOVING);
  ck_assert_int_eq(gp->f_pos.y, START_FIGURE_Y);
  ck_assert_int_eq(gp->f_pos.x, START_FIGURE_X);

  ck_assert_int_eq(gi.pause, NoPause);

  freeGameBackend();
}
END_TEST

START_TEST(action_Down_2) {
  int high_score = 12345;
  initGameBackend(high_score);

  userInput(Start, false);
  for (int i = 0; i < FIELD_N + 2; i++) userInput(Down, false);
  GameParams_t *gp = getStaticGameParams();
  GameInfo_t gi = updateCurrentState();

  ck_assert_int_eq(gp->pause, GameOverStatePause);
  ck_assert_int_eq(gp->state, GAMEOVER);

  ck_assert_int_eq(gi.pause, GameOverStatePause);

  freeGameBackend();
}
END_TEST

START_TEST(action_Action_1) {  // Rotate
  int high_score = 12345;
  initGameBackend(high_score);

  userInput(Start, false);
  userInput(Action, false);
  GameParams_t *gp = getStaticGameParams();
  GameInfo_t gi = updateCurrentState();

  ck_assert_int_eq(gp->pause, NoPause);
  ck_assert_int_eq(gp->state, MOVING);
  ck_assert_int_eq(gp->f_pos.y, START_FIGURE_Y);
  ck_assert_int_eq(gp->f_pos.x, START_FIGURE_X);
  ck_assert_int_eq(gp->f_orient, F_ORIENT_0 + 1);

  ck_assert_int_eq(gi.pause, NoPause);

  freeGameBackend();
}
END_TEST

START_TEST(doShifting_1) {
  int high_score = 12345;
  initGameBackend(high_score);

  userInput(Start, false);
  GameParams_t *gp = getStaticGameParams();
  sleep(getShiftingInterval(gp) / 1000 + 1);
  GameInfo_t gi = updateCurrentState();

  ck_assert_int_eq(gp->pause, NoPause);
  ck_assert_int_eq(gp->state, MOVING);
  ck_assert_int_eq(gp->f_pos.y, START_FIGURE_Y + 1);
  ck_assert_int_eq(gp->f_pos.x, START_FIGURE_X);

  ck_assert_int_eq(gi.pause, NoPause);

  freeGameBackend();
}
END_TEST

START_TEST(doShifting_2) {
  int high_score = 12345;
  initGameBackend(high_score);
  GameParams_t *gp = getStaticGameParams();
  GameInfo_t gi = updateCurrentState();

  userInput(Start, false);
  for (int j = 0; j < FIELD_M; j++) {
    gp->field[FIELD_N - 1][j] = PIXEL_FILLED;
  }

  for (int i = 0; i < 8; i++) userInput(Down, false);
  gp->level = LVL_MAX;
  for (int i = 0; i < 12 && gp->state != GAMEOVER; i++) {
    sleep(getShiftingInterval(gp) / 1000 + 1);
    gi = updateCurrentState();
  }

  ck_assert_int_eq(gp->pause, GameOverStatePause);
  ck_assert_int_eq(gp->state, GAMEOVER);

  ck_assert_int_eq(gi.pause, GameOverStatePause);
  ck_assert_int_eq(gi.score, POINTS_1_LINE);

  freeGameBackend();
}
END_TEST

// ==============================================================
// Suite setup
// ==============================================================
Suite *get_suite_tetris(void) {
  Suite *s = suite_create("tetris_suite");

  TCase *tc = tcase_create("tc");

  tcase_add_test(tc, init_1);
  tcase_add_test(tc, action_Start_1);
  tcase_add_test(tc, action_Pause_1);
  tcase_add_test(tc, action_Left_1);
  tcase_add_test(tc, action_Right_1);
  tcase_add_test(tc, action_Down_1);
  tcase_add_test(tc, action_Down_2);
  tcase_add_test(tc, action_Action_1);
  tcase_add_test(tc, doShifting_1);
  tcase_add_test(tc, doShifting_2);

  suite_add_tcase(s, tc);

  return s;
}