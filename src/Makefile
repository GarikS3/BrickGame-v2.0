CC = gcc
CPP = g++
CFLAGS = -Wall -Wextra -Werror -std=c11 -I./src
CPPFLAGS = -Wall -Wextra -Werror -std=c++17 -I./src
LDFLAGS_CHECK = -lcheck -lm -lpthread -lncurses
LDFLAGS_GTEST = -lgtest -lgtest_main -pthread
GCOV_FLAGS = --coverage -fprofile-arcs -ftest-coverage
TEST_FLAGS = -L$(LIB_BUILD_DIR) -ltetris -lsnake

OS_NAME = $(shell uname)
ifeq ($(OS_NAME),Darwin)
    OPEN = open
endif
ifeq ($(OS_NAME),Linux)
	LDFLAGS_CHECK += -lrt -lsubunit
    OPEN = xdg-open
endif

LIB_BUILD_DIR = build/lib
CLI_BUILD_DIR = build/cli
DESKTOP_BUILD_DIR = build/desktop
OBJ_DIR = build/obj
DIST_DIR = s21_Brick_game

TETRIS_LIB = $(LIB_BUILD_DIR)/libtetris.a
SNAKE_LIB = $(LIB_BUILD_DIR)/libsnake.a
CLI_TARGET = $(CLI_BUILD_DIR)/brickgame_cli
DESKTOP_TARGET = $(DESKTOP_BUILD_DIR)/brickgame_desktop

TETRIS_SRC = $(wildcard brick_game/tetris/*.c) \
			gui/cli/tetris_console.c
SNAKE_SRC = brick_game/snake/model/model.cpp \
            brick_game/snake/controller/controller.cpp
CLI_SRC = $(wildcard brick_game/tetris/*.c) \
	brick_game/snake/controller/controller.cpp \
    brick_game/snake/model/model.cpp \
	$(wildcard gui/cli/*.c) \
	$(wildcard gui/cli/*.cpp)

TETRIS_OBJ = $(patsubst %.c,$(OBJ_DIR)/%.o,$(TETRIS_SRC))
SNAKE_OBJ = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SNAKE_SRC))
CLI_OBJ = $(patsubst %.c,$(OBJ_DIR)/%.o,$(filter %.c,$(CLI_SRC))) \
          $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(filter %.cpp,$(CLI_SRC)))

PRO_FILE = gui/desktop/brick_game.pro
QMAKE = qmake

.PHONY: all install uninstall clean run_cli run_desktop dvi

all: clean install test

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CPP) $(CPPFLAGS) -c $< -o $@

$(TETRIS_LIB): $(TETRIS_OBJ)
	@mkdir -p $(@D)
	ar rcs $@ $^

$(SNAKE_LIB): $(SNAKE_OBJ)
	@mkdir -p $(@D)
	ar rcs $@ $^

$(CLI_TARGET): $(CLI_OBJ) $(TETRIS_LIB) $(SNAKE_LIB)
	@mkdir -p $(@D)
	$(CPP) $^ -o $@ -lncurses
	@rm -rf $(OBJ_DIR)

$(DESKTOP_TARGET):
	@mkdir -p $(DESKTOP_BUILD_DIR)
	@cd $(DESKTOP_BUILD_DIR) && $(QMAKE) ../../$(PRO_FILE)
	@cd $(DESKTOP_BUILD_DIR) && make
	@cd $(DESKTOP_BUILD_DIR) && rm -f moc_*.cpp *.moc qrc_*.cpp ui_*.h *.h *.o Makefile

install: $(CLI_TARGET) $(DESKTOP_TARGET)
	@echo 0 > $(CLI_BUILD_DIR)/tetris_high_score.txt
	@echo 0 > $(CLI_BUILD_DIR)/snake_high_score.txt
	@echo 0 > $(DESKTOP_BUILD_DIR)/tetris_high_score.txt
	@echo 0 > $(DESKTOP_BUILD_DIR)/snake_high_score.txt

uninstall:
	rm -rf build

run_cli:
	@cd $(CLI_BUILD_DIR) && ./brickgame_cli

run_desktop:
ifeq ($(OS_NAME),Darwin)
	@cd $(DESKTOP_BUILD_DIR) && ./brickgame_desktop.app/Contents/MacOS/brickgame_desktop;	
endif
ifeq ($(OS_NAME),Linux)
	@cd $(DESKTOP_BUILD_DIR) && ./brickgame_desktop
endif

test: install
	@$(CC) $(CFLAGS) $(wildcard tests/*.c) $(TEST_FLAGS) $(LDFLAGS_CHECK) -o tests/tetris_tests
	@$(CPP) $(CPPFLAGS) $(wildcard tests/*.cpp) $(TEST_FLAGS) $(LDFLAGS_GTEST) -o tests/snake_tests
	@echo 0 > tests/tetris_high_score.txt
	@echo 0 > tests/snake_high_score.txt
	@cd tests && ./tetris_tests
	@cd tests && ./snake_tests

gcov_report:
	@$(CC) $(CFLAGS) --coverage $(wildcard brick_game/tetris/*.c) \
		$(wildcard gui/cli/*.c) tests/tetris_tests.c \
		$(LDFLAGS_CHECK) -o tests/tetris_gcov_tests
	@$(CPP) $(CPPFLAGS) --coverage brick_game/snake/model/model.cpp \
		brick_game/snake/controller/controller.cpp tests/snake_tests.cpp \
		$(LDFLAGS_GTEST) -o tests/snake_gcov_tests
	@cd tests && ./tetris_gcov_tests
	@cd tests && ./snake_gcov_tests
	@mkdir -p coverage_report

ifeq ($(OS_NAME),Darwin)
	gcovr -r . --html --html-details --output coverage_report/index.html
endif
ifeq ($(OS_NAME),Linux)
	lcov --capture --directory . --output-file coverage.info --ignore-errors mismatch 2>/dev/null
	lcov --remove coverage.info '*/usr/include/*' '*/tests/*' -o filtered.info 2>/dev/null
	genhtml filtered.info --output-directory coverage_report 2>/dev/null
endif

	$(OPEN) coverage_report/index.html

dvi:
	latex -output-directory=doc doc/documentation.tex
	dvips -o doc/documentation.ps doc/documentation.dvi
	ps2pdf doc/documentation.ps doc/documentation.pdf
	rm -f doc/*.aux doc/*.log doc/*.toc doc/*.out doc/*.ps

dist:
	mkdir -p $(DIST_DIR)
	cp -a brick_game $(DIST_DIR)
	cp -a gui $(DIST_DIR)
	cp -a doc $(DIST_DIR)
	cp -a tests $(DIST_DIR)
	tar -czf Brickgames.tar.gz $(DIST_DIR)
	rm -rf $(DIST_DIR)

leaks: test
ifeq ($(OS_NAME),Darwin)
	@cd tests && leaks --atExit -- ./tetris_tests
	@cd tests && leaks --atExit -- ./snake_tests
endif
ifeq ($(OS_NAME),Linux)
	@cd tests && valgrind --leak-check=full --show-leak-kinds=definite ./tetris_tests
	@cd tests && valgrind --leak-check=full --show-leak-kinds=definite ./snake_tests
endif

clang_check:
	cp ../materials/linters/.clang-format .clang-format
	clang-format -n brick_game/tetris/*.c brick_game/tetris/*.h
	clang-format -n brick_game/snake/model/*.cpp brick_game/snake/model/*.h
	clang-format -n brick_game/snake/controller/*.cpp brick_game/snake/controller/*.h
	clang-format -n gui/cli/*.c gui/cli/*.h gui/cli/*.cpp
	clang-format -n gui/desktop/*.cpp gui/desktop/*.h
	clang-format -n tests/*.c tests/*.cpp
	rm -f .clang-format

clang_format:
	cp ../materials/linters/.clang-format .clang-format
	clang-format -i brick_game/tetris/*.c brick_game/tetris/*.h
	clang-format -i brick_game/snake/model/*.cpp brick_game/snake/model/*.h
	clang-format -i brick_game/snake/controller/*.cpp brick_game/snake/controller/*.h
	clang-format -i gui/cli/*.c gui/cli/*.h gui/cli/*.cpp
	clang-format -i gui/desktop/*.cpp gui/desktop/*.h
	clang-format -i tests/*.c tests/*.cpp
	rm -f .clang-format

clean:
	rm -rf build
	rm -f tests/tetris_tests tests/snake_tests tests/tetris_gcov_tests tests/snake_gcov_tests
	rm -f tests/*.gcda tests/*.gcno *.gcov tests/*.txt
	rm -rf coverage_report
	rm -f doc/*.dvi doc/*.pdf
	rm -f *.info
	rm -rf Brickgames.tar.gz

rebuild: clean install