#ifndef S21_CAT_H
#define S21_CAT_H

#include <getopt.h>
#include <stdio.h>

typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} Flags;

typedef struct {
  int count_lines;
  int count_newlines;
} Count;

typedef struct {
  char ch_before, ch;
} Chars;

const struct option long_options[] = {
    {"number-nonblank", no_argument, NULL, 'b'},
    {"number", no_argument, NULL, 'n'},
    {"squeeze-blank", no_argument, NULL, 's'},
    {"show-ends", no_argument, NULL, 'E'},
    {"show-tabs", no_argument, NULL, 'T'},
    {"show-nonprinting", no_argument, NULL, 'v'},
};

int parse_flags(const int argc, char *argv[], Flags *flags,
                int *files_argv_index);
FILE *handle_flags(const char *filename, Flags flags, Count *count,
                   Chars *chars);
void b_flag(char ch_before, char ch, int *count_line);
void e_flag(char ch);
int n_flag(char ch, int *count_lines);
int s_flag(char ch_before, char ch, int *count_newline, int *count_lines,
           int flags_n, int flags_b);
int v_flag(char ch);
int t_flag(char ch);

#endif