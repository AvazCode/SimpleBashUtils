CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror
TARGET = s21_cat

all: $(TARGET)

$(TARGET): s21_cat.c
	$(CC) $(CFLAGS) -o $(TARGET) s21_cat.c

clean:
	rm -f s21_cat
