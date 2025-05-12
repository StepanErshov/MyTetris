SHELL = /bin/sh

CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c11 
TFLAGS= $(shell pkg-config --cflags --libs check) -lpthread -lm
AFLAGS= -lncurses
BUILD=./
INSTALL_DIR=./install
DIST_DIR= ./BrickGame_v1
DIST_ARCHIVE= tetris.tar.gz
DVI_DIR=./dvi

TETRIS_LIB=libtetris.a
TETRIS_LIB_DIR=./brick_game/tetris
TETRIS_ALL_C=$(TETRIS_LIB_DIR)/*.c 
TETRIS_ALL_H=$(TETRIS_LIB_DIR)/*.h 

APP=tetris_app
APP_DIR=./gui/cli
APP_ALL_C= $(APP_DIR)/*.c 
APP_ALL_H=$(APP_DIR)/*.h 

TESTS_RUNNER=brick_game_tests_runner
TESTS_DIR=./tests
TESTS_ALL_C=$(TESTS_DIR)/*.c
TESTS_ALL_H=$(TESTS_DIR)/brick_game_tests.h

PRJ_CODE_ALL=$(TETRIS_ALL_C) $(TETRIS_ALL_H) $(TESTS_ALL_C) $(TESTS_ALL_H) 
PRJ_CODE_ALL += $(APP_ALL_C) $(APP_ALL_H)

VALGRIND=valgrind --tool=memcheck --leak-check=yes
# --quiet --track-origins=yes --leak-check=full -v

OS:=$(shell uname -s)
ifeq ($(OS), Linux)
TFLAGS += -lsubunit -lrt 
endif

.PHONY: all install uninstall dist dvi $(TETRIS_LIB) $(APP) test clean clean_all clang gcov_report lcov_report valgrind 

all: $(APP)

install: $(APP)
	mkdir -p $(INSTALL_DIR)
	mv $(BUILD)$(APP) $(INSTALL_DIR)

uninstall:
	rm -rf $(INSTALL_DIR)

dist: install
	mkdir -p $(DIST_DIR) $(DIST_DIR)/gui $(DIST_DIR)/brick_game
	mv $(INSTALL_DIR)  $(DIST_DIR)
	cp -r $(TETRIS_LIB_DIR)  $(DIST_DIR)/brick_game
	cp -r $(APP_DIR)  $(DIST_DIR)/gui
	cp -r $(TESTS_DIR)  $(DIST_DIR)
	cp  Makefile $(DIST_DIR)
	cp  FSM_Diagram.png $(DIST_DIR)
	cp  readme.txt $(DIST_DIR)
	cp  readme.texi $(DIST_DIR)
	tar -czvf $(DIST_ARCHIVE) $(DIST_DIR)
	-rm -rf $(DIST_DIR)

dvi:
	mkdir -p $(DVI_DIR)
	texi2dvi readme.texi --dvi -o readme.dvi
	mv readme.dvi $(DVI_DIR)
	mv readme.aux $(DVI_DIR)
	mv readme.log $(DVI_DIR)
	mv readme.toc $(DVI_DIR)
	# mv dvi/readme.txt .
	# mv dvi/readme.texi .


$(TETRIS_LIB): clean
	$(CC) $(CFLAGS) -c $(TETRIS_ALL_C) 
	ar rcs $(BUILD)$(TETRIS_LIB) ./*.o
	ranlib $(BUILD)$(TETRIS_LIB) 
	rm -f ./*.o

$(APP): clean $(TETRIS_LIB)
	$(CC) $(CFLAGS) $(APP_ALL_C) $(BUILD)$(TETRIS_LIB)  $(AFLAGS) -lSDL2 -lSDL2_mixer -o $(BUILD)$(APP)
	rm -f $(BUILD)$(TETRIS_LIB)
	# $(BUILD)$(APP)

test: clean $(TETRIS_LIB)
	$(CC) $(CFLAGS) $(TESTS_ALL_C) $(BUILD)$(TETRIS_LIB) $(TFLAGS) -o $(BUILD)$(TESTS_RUNNER)
	$(BUILD)$(TESTS_RUNNER)
 
clean:
	rm -f $(BUILD)*.o $(BUILD)$(TETRIS_LIB) $(BUILD)$(APP) $(BUILD)$(TESTS_RUNNER) \
		  $(BUILD)*.gcda $(BUILD)*.gcno $(BUILD)*.gcov $(BUILD)high_score.txt
	rm -rf $(BUILD)report_lcov
	rm -rf $(BUILD)report_gcov


clean_all: clean uninstall 
	rm -rf $(DVI_DIR)
	rm -f $(DIST_ARCHIVE)

clang:
	cp ../materials/linters/.clang-format ./ 
	clang-format -i $(PRJ_CODE_ALL)
	clang-format -n $(PRJ_CODE_ALL)
	rm -f ./.clang-format

lcov_report:
	$(CC) $(CFLAGS) --coverage $(TESTS_ALL_C) $(TETRIS_ALL_C) \
							$(TFLAGS) -lSDL2 -lSDL2_mixer -o $(BUILD)$(TESTS_RUNNER)			
	$(BUILD)$(TESTS_RUNNER)

	rm -rf $(BUILD)report_lcov
	mkdir $(BUILD)report_lcov
	lcov -t "$(TESTS_RUNNER)" -c -d $(BUILD) \
					-o $(BUILD)report_lcov/$(TESTS_RUNNER)_report_lcov.info 
	genhtml -o $(BUILD)report_lcov $(BUILD)report_lcov/$(TESTS_RUNNER)_report_lcov.info
	open $(BUILD)report_lcov/index.html
	rm -f $(BUILD)*.gcda $(BUILD)*.gcno

gcov_report:
	$(CC) $(CFLAGS) --coverage $(TESTS_ALL_C) $(TETRIS_ALL_C) \
							$(TFLAGS) -lSDL2 -lSDL2_mixer -o $(BUILD)$(TESTS_RUNNER)			
	$(BUILD)$(TESTS_RUNNER)
	gcov -o  $(BUILD)*.gcno
	rm -rf $(BUILD)report_gcov
	mkdir $(BUILD)report_gcov
	gcovr -r $(BUILD) --exclude 'tests/' --html --html-details \
				-o $(BUILD)report_gcov/$(TESTS_RUNNER)_report_gcov.html
	open $(BUILD)report_gcov/$(TESTS_RUNNER)_report_gcov.html
	rm -f $(BUILD)*.gcda $(BUILD)*.gcno $(BUILD)*.gcov

valgrind: test
	$(VALGRIND) $(BUILD)$(TESTS_RUNNER)


# install ncurses: sudo apt-get install libncurses5-dev libncursesw5-dev
# install texi2dvi: sudo apt install texinfo
# install TeX: sudo apt install texlive
