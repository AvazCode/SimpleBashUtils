#include "s21_cat.h"

int main(int argc, char *argv[]) {
  if (argc > 1) {
    Flags flags = {0};
    Count count = {0};
    Chars chars = {0};

    chars.ch_before = '\n';

    int files_argv_index = 1;
    if (parse_flags(argc, argv, &flags, &files_argv_index) == -1) {
      fprintf(stderr, "Invalid flag!\n");
      return 1;
    }

    for (int i = files_argv_index; i < argc; i++) {
      fclose(handle_flags(argv[i], flags, &count, &chars));
    }
  }

  return 0;
}

void b_flag(char ch_before, char ch, int *count_line) {
  if ((ch_before == '\n') && (ch != '\n')) {
    fprintf(stdout, "%6d\t", ++(*count_line));
  }
}

void e_flag(char ch) {
  if (ch == '\n') {
    putchar('$');
  }
}

int n_flag(char ch, int *count_lines) {
  int carry_on = 0;

  if (*count_lines == 0) {
    fprintf(stdout, "%6d\t", ++(*count_lines));
  }

  if (ch == '\n' || (*count_lines == 0)) {
    fprintf(stdout, "\n%6d\t", ++(*count_lines));
    carry_on = 1;
  }

  return carry_on;
}

int s_flag(char ch_before, char ch, int *count_newline, int *count_lines,
           int flags_n, int flags_b) {
  int carry_on = 0;

  if ((ch_before == '\n') && (ch == '\n')) {
    (*count_newline)++;
  } else {
    *count_newline = 0;
  }

  if (*count_newline > 1 && !carry_on) {
    carry_on = 1;
  }

  if (flags_n && !flags_b && (ch_before == '\n' && carry_on == 0)) {
    fprintf(stdout, "%6d\t", ++(*count_lines));
  }

  if ((flags_b && (ch_before == '\n') && (ch != '\n'))) {
    fprintf(stdout, "%6d\t", ++(*count_lines));
  }

  return carry_on;
}

int v_flag(char ch) {
  int carry_on = 0;
  if ((ch >= 0 && ch < 32) && (ch != '\n') && (ch != '\t')) {
    fprintf(stdout, "^%c", ch + 64);
    carry_on = 1;
  } else if (ch == 127) {
    fprintf(stdout, "^?");
    carry_on = 1;
  } else if (ch >= 127) {
    fprintf(stdout, "M-");
    ch -= 128;
    if (ch < 32) {
      fprintf(stdout, "^%c", ch + 64);
    } else {
      putchar(ch);
    }
    carry_on = 1;
  }

  return carry_on;
}

int t_flag(char ch) {
  int carry_on = 0;

  if (ch == '\t') {
    printf("^I");
    carry_on = 1;
  } else {
    carry_on =
        0;  // Tab belgisidan boshqa belgilar uchun hech narsa qilinmaydi.
  }

  return carry_on;
}

FILE *handle_flags(const char *filename, Flags flags, Count *count,
                   Chars *chars) {
  FILE *file;

  file = fopen(filename, "r");

  if (file == NULL) {
    fprintf(stderr, "The file does not exist: %s\n", filename);
    return NULL;
  }

  while ((chars->ch = fgetc(file)) != EOF) {
    if (flags.s) {
      if (s_flag(chars->ch_before, chars->ch, &count->count_newlines,
                 &count->count_lines, flags.n, flags.b) == 1) {
        continue;
      }
    }
    if (flags.b == 1) {
      if (flags.n && !flags.s) {
        b_flag(chars->ch_before, chars->ch, &count->count_lines);
      }
      if (!flags.n && !flags.s) {
        b_flag(chars->ch_before, chars->ch, &count->count_lines);
      }
    }
    if (flags.e == 1) {
      e_flag(chars->ch);
    }
    if ((flags.n == 1) && ((flags.b == 0) && (flags.s == 0))) {
      if (n_flag(chars->ch, &count->count_lines) == 1) {
        continue;
      }
    }
    if (flags.v == 1) {
      if (v_flag(chars->ch) == 1) {
        continue;
      }
    }
    if (flags.t == 1) {
      if (t_flag(chars->ch) == 1) {
        continue;
      }
    }
    chars->ch_before = chars->ch;
    putchar(chars->ch);
  }

  return file;
}

int parse_flags(const int argc, char **argv, Flags *flags,
                int *files_argv_index) {
  for (int i = 1; (i < argc && argv[i][0] == '-'); i++, *files_argv_index = i) {
    int flag = getopt_long(argc, argv, "+bnsevtET", long_options, NULL);
    switch (flag) {
      case 'b':
        flags->b = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 'e':
        flags->e = 1;
        flags->v = 1;
        break;
      case 'E':
        flags->e = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 't':
        flags->t = 1;
        flags->v = 1;
        break;
      case 'T':
        flags->t = 1;
        break;
      case '?':
        return -1;
    }
  }

  return 0;
}