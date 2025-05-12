#include <stdio.h>
#include <stdlib.h>

#include "brick_game_tests.h"

int main() {
  int failed_tests = 0;

  Suite *s = get_suite_tetris();
  SRunner *sr = srunner_create(s);
  // srunner_add_suite(sr, get_suite_tetris());

  srunner_set_fork_status(sr, CK_NOFORK);  // for valgrind chechs
  srunner_run_all(sr, CK_VERBOSE);
  failed_tests = srunner_ntests_failed(sr);
  printf("=== FAILED TESTS: %d ===\n", failed_tests);

  srunner_free(sr);
  return (failed_tests == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}