CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -Werror -Wpedantic

GCOV_FLAGS := -fprofile-arcs -ftest-coverage

OS := $(shell uname -s)

ifeq ($(OS), Darwin)
	LIB_FLAGS := $(shell pkg-config --static --cflags --libs check ncurses)
else
	LIB_FLAGS := $(shell pkg-config --static --cflags --libs check ncursesw)
endif

PROJECT := tetris_app
GUI := gui/frontend.c
BUILD_DIR := build
DOCS_DIR := docs
TESTS_DIR := tests
REPORT_DIR := report
DIST_DIR := dist

all : install


install : tetris.a | build_dir
	$(CC) $(CFLAGS) $(PROJECT).c $(GUI) $(BUILD_DIR)/tetris.a -o $(BUILD_DIR)/$(PROJECT) $(LIB_FLAGS)

tetris.a : build_dir
	$(CC) $(CFLAGS) -c brick_game/tetris/tetris.c -o $(BUILD_DIR)/tetris.o
	ar rc $(BUILD_DIR)/tetris.a $(BUILD_DIR)/tetris.o
	ranlib $(BUILD_DIR)/tetris.a
	rm -rf $(BUILD_DIR)/tetris.o

uninstall :
	rm -rf $(DOCS_DIR) $(BUILD_DIR)

rebuild : uninstall install


clean :
	rm -rf $(DOCS_DIR) $(BUILD_DIR) $(DIST_DIR) $(REPORT_DIR) $(TESTS_DIR)/tetris_test

dvi :
	doxygen Doxyfile

dist : dist_dir
	tar cf $(DIST_DIR)/tetris-1.0.tar $(BUILD_DIR) $(DOCS_DIR)

test :
	$(CC) $(CFLAGS) $(TESTS_DIR)/front_test.c $(TESTS_DIR)/tetris_test.c $(BUILD_DIR)/tetris.a -o $(TESTS_DIR)/tetris_test $(LIB_FLAGS)
	./$(TESTS_DIR)/tetris_test
	
gcov_report : build_dir
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c brick_game/tetris/tetris.c -o $(BUILD_DIR)/tetris.o $(LIB_FLAGS)
	ar rc $(BUILD_DIR)/tetris.a $(BUILD_DIR)/tetris.o
	ranlib $(BUILD_DIR)/tetris.a
	rm -rf $(BUILD_DIR)/tetris.o
	$(CC) $(CFLAGS) $(TESTS_DIR)/front_test.c $(TESTS_DIR)/tetris_test.c $(BUILD_DIR)/tetris.a -o $(TESTS_DIR)/tetris_test $(LIB_FLAGS) -lgcov
	./$(TESTS_DIR)/tetris_test
	rm -rf $(REPORT_DIR)
	mkdir $(REPORT_DIR)
	gcovr --html-details $(REPORT_DIR)/coverage.html
	rm -rf *.gcda *.gcno *.o
	
format :
	clang-format --style=file:"../materials/linters/.clang-format" -i *.c $(TESTS_DIR)/*.c $(TESTS_DIR)/*.h */*/*.c */*/*.h

check :
	clang-format --style=file:"../materials/linters/.clang-format" -n  *.c */*.c */*.h */*/*.c */*/*.h

valgrind-test :
	CK_FORK=no valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all -s --track-origins=yes $(TESTS_DIR)/tetris_test

build_dir :
	mkdir -p $(BUILD_DIR)

dist_dir :
	mkdir -p $(DIST_DIR)