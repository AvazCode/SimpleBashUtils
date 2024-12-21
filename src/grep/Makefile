CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror
TARGET = s21_grep

all: $(TARGET)

$(TARGET): s21_grep.c
	$(CC) $(CFLAGS) -o $(TARGET) s21_grep.c

clean:
	rm -f s21_grep
