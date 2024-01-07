CC=gcc
FLAGS=-Wall -Werror -Wextra -std=c11
MAC_FLAGS=-lcheck -lm
LINUX_FLAGS=-lcheck -lm -lrt -lpthread -lsubunit
GCOV_FLAGS=-fprofile-arcs -ftest-coverage
VALGRIND_FLAGS_v2 = --log-file="valgrind.txt" --track-origins=yes --trace-children=yes --leak-check=full --leak-resolution=med #--track-fds=yes --show-leak-kinds=all

VALGRIND_FLAGS=--leak-check=full

FILES=$(wildcard s21_*.c)
OBJ=$(patsubst %o, %o, $(patsubst %.c,%.o,$(FILES)))
TEST_FILES = $(wildcard tests/test_*.c)
USERNAME=$(shell whoami)

MYLIB = s21_string.a

OS := $(shell uname -s)

ifeq ($(OS),Darwin)
	OPEN=open
endif
ifeq ($(OS),Linux)
	OPEN=xdg-open
endif

all: s21_string.a

.PHONY: all test memory_test s21_string.a gcov_report clean rebuild

s21_string.a: $(FILES)
	$(CC) -c $(FLAGS) $(FILES)
	ar rc $@ $(OBJ)
	ranlib $@
	rm -rf *.o

test: s21_string.a
ifeq ($(OS),Darwin)
	$(CC) $(FLAGS) $(TEST_FILES) $< -o test $(MAC_FLAGS)
else
	$(CC) $(FLAGS) $(TEST_FILES) $< -o test $(LINUX_FLAGS)
endif
	./test

# вызываем, если у нас есть lcov -> на выводе у нас html страница с покрытием тестов
gcov_report: clean
ifeq ($(OS),Darwin)
	$(CC) $(FLAGS) $(GCOV_FLAGS) $(FILES) $(TEST_FILES) -o test $(MAC_FLAGS)
else
	$(CC) $(FLAGS) $(GCOV_FLAGS) $(FILES) $(TEST_FILES) -o test $(LINUX_FLAGS)
endif
	./test
	rm *test_*
	lcov -t "s21_test" -o postreport.info -c -d .
	genhtml -o report postreport.info
	$(OPEN) report/index.html

install_brew: # устанавливаем brew в случае, если его нет
	git clone https://github.com/Homebrew/brew /opt/goinfre/$(USERNAME)/homebrew
	eval /opt/goinfre/$(USERNAME)/homebrew/bin/brew shellenv
	/opt/goinfre/$(USERNAME)/homebrew/bin/brew  update --force --quiet

lcov: # с помощью brew подгружаем пакет lcov
	/opt/goinfre/$(USERNAME)/homebrew/bin/brew install lcov

style_check:
	clang-format -i -style=google tests/*.c tests/*.h *.c *.h
	clang-format -n -style=google tests/*.c tests/*.h *.c *.h

memory_check: s21_string.a
	$(CC) $(FLAGS) $(TEST_FILES) $< -o memory_test $(LINUX_FLAGS)
	CK_FORK=no valgrind $(VALGRIND_FLAGS) ./memory_test
	
valgrind: 
	valgrind $(VALGRIND_FLAGS_v2) ./test
	grep -n "ERROR SUMMARY" valgrind.txt | grep -v "0 errors"

# очистка src
clean_obj:
	rm -rf obj/*.o *.o

clean_lib: 
	rm -rf *.a

clean_test:
	rm -rf *.gcda
	rm -rf *.gcno
	rm -rf *.info
	rm -rf test
	rm -rf report
	rm -rf valgrind.txt

clean: clean_lib clean_test clean_obj
	rm -rf unit_test
	rm -rf RESULT_VALGRIND.txt

rebuild: clean all
