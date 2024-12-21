#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_usage() {
  printf("Usage: s21_cat [OPTIONS] [FILE]...\n");
  printf("OPTIONS:\n");
  printf("  -b    Number non-empty output lines\n");
  printf("  -n    Number all output lines\n");
  printf("  -s    Squeeze multiple blank lines into one\n");
  exit(1);
}

int main(int argc, char *argv[]) {
  int opt;
  int number_lines = 0, number_nonempty = 0, squeeze_blank = 0;
  FILE *file;
  char line[1024];
  int line_num = 0, last_was_blank = 0;

  // Parse options
  while ((opt = getopt(argc, argv, "bns")) != -1) {
    switch (opt) {
      case 'b':
        number_nonempty = 1;
        break;
      case 'n':
        number_lines = 1;
        break;
      case 's':
        squeeze_blank = 1;
        break;
      default:
        print_usage();
    }
  }

  // Open and read files
  for (int i = optind; i < argc; i++) {
    file = fopen(argv[i], "r");
    if (!file) {
      perror("Error opening file");
      continue;
    }

    while (fgets(line, sizeof(line), file)) {
      int is_blank = (line[0] == '\n');
      if (squeeze_blank && is_blank && last_was_blank) continue;
      last_was_blank = is_blank;

      if (number_lines)
        printf("%6d\t", ++line_num);
      else if (number_nonempty && !is_blank)
        printf("%6d\t", ++line_num);

      printf("%s", line);
    }

    fclose(file);
  }

  return 0;
}
