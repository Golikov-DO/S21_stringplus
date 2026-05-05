CC = gcc

CFLAGS = -Wall -Wextra -Werror -std=c11 -D_POSIX_C_SOURCE=200809L
TEST_FLAGS = -lcheck -lsubunit -lm -lpthread
GCOV_FLAGS = --coverage

SRC = s21_string.c s21_sscanf.c s21_sprintf.c \
      s21_sprintf_utils.c s21_sprintf_float.c

OBJ = $(SRC:.c=.o)

TEST_SRC = test/test.c

LIB = s21_string.a
TEST_EXE = test_exec

all: $(LIB)

$(LIB): $(OBJ)
	ar rcs $(LIB) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

build_test: $(LIB)
	$(CC) $(CFLAGS) -I. $(TEST_SRC) $(LIB) $(TEST_FLAGS) -o $(TEST_EXE)

test: build_test
	./$(TEST_EXE)

memcheck: build_test
	valgrind --tool=memcheck --leak-check=full ./$(TEST_EXE)

gcov_report: clean
	$(CC) $(CFLAGS) -I. $(GCOV_FLAGS) $(SRC) $(TEST_SRC) $(TEST_FLAGS) -o $(TEST_EXE)
	./$(TEST_EXE)
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory report

clean:
	rm -rf *.o *.a *.gcno *.gcda *.info report $(TEST_EXE)