#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_usage() {
  printf("Usage: s21_grep [OPTIONS] PATTERN [FILE]...\n");
  printf("OPTIONS:\n");
  printf("  -e PATTERN  Use PATTERN as the matching pattern\n");
  printf("  -i          Ignore case distinctions\n");
  printf("  -v          Invert match\n");
  printf("  -c          Print count of matching lines\n");
  printf("  -l          Print file names with matches\n");
  printf("  -n          Print line numbers with matches\n");
  exit(1);
}

int main(int argc, char *argv[]) {
  int opt, ignore_case = 0, invert_match = 0, count_only = 0,
           print_filenames = 0, line_numbers = 0;
  char *pattern = NULL;
  regex_t regex;
  int reti;

  while ((opt = getopt(argc, argv, "e:ivcln")) != -1) {
    switch (opt) {
      case 'e':
        pattern = optarg;
        break;
      case 'i':
        ignore_case = REG_ICASE;
        break;
      case 'v':
        invert_match = 1;
        break;
      case 'c':
        count_only = 1;
        break;
      case 'l':
        print_filenames = 1;
        break;
      case 'n':
        line_numbers = 1;
        break;
      default:
        print_usage();
    }
  }

  if (!pattern) {
    fprintf(stderr, "Pattern must be specified with -e\n");
    return 1;
  }

  reti = regcomp(&regex, pattern, REG_EXTENDED | ignore_case);
  if (reti) {
    fprintf(stderr, "Could not compile regex\n");
    return 1;
  }

  for (int i = optind; i < argc; i++) {
    FILE *file = fopen(argv[i], "r");
    if (!file) {
      perror("Error opening file");
      continue;
    }

    char line[1024];
    int line_num = 0, match_count = 0;

    while (fgets(line, sizeof(line), file)) {
      line_num++;
      reti = regexec(&regex, line, 0, NULL, 0);
      int match = (reti == 0) ^ invert_match;

      if (match) {
        if (count_only) {
          match_count++;
        } else if (print_filenames) {
          printf("%s\n", argv[i]);
          break;
        } else {
          if (line_numbers) printf("%d:", line_num);
          printf("%s", line);
        }
      }
    }

    if (count_only) printf("%d\n", match_count);
    fclose(file);
  }

  regfree(&regex);
  return 0;
}
