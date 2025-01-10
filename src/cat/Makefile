COMPILER = gcc

FLAGS = -Wall -Wextra -Werror -std=c11

INPUT_FILES = s21_cat.c

OUTPUT_FILE = s21_cat

all: s21_cat

s21_cat: Makefile s21_cat.h s21_cat.c
	$(COMPILER) $(FLAGS) $(INPUT_FILES) -o $(OUTPUT_FILE)

check:
	clang-format -style=Google -n *.c *.h

rebuild:
	make clean
	make all

clean:
	rm -f s21_cat


