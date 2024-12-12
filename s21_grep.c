#include "s21_grep.h"

int main(int argc, char **argv) {
  flags flag = {0};

  char pattern[4096] = "";
  parse_flag(argc, argv, &flag, pattern);
  if (strlen(pattern) == 0) {
    fprintf(stderr, "Error: No pattern provided\n");
    return 1;
  }
  int multiple_files = (argc - optind > 1);
  for (int i = optind; i < argc; i++) {
    print_file(argv[i], pattern, flag, multiple_files);
  }

  return 0;
}
void add_pattern(char *pattern, char *tmpl) {
  if (strlen(pattern) > 0) {
    pattern[strlen(pattern)] = '|';
    strcat(pattern, tmpl);
  } else {
    strcat(pattern, tmpl);
  }
}

void pattern_file(char *name, char *pattern, int flag) {
  FILE *file = fopen(name, "r");
  if (file == NULL) {
    if (!flag) {
      fprintf(stderr, "Error: Cannot open file '%s'\n", name);
    }
    return;
  }
  char line[4096];
  while (fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = '\0';
    add_pattern(pattern, line);
  }
  fclose(file);
}

void parse_flag(int argc, char **argv, flags *flag, char *pattern) {
  int res;
  while ((res = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, NULL)) != -1) {
    switch (res) {
      case 'e':
        flag->e = 1;
        add_pattern(pattern, optarg);
        break;
      case 'i':
        flag->i = 1;
        break;
      case 'v':
        flag->v = 1;
        break;
      case 'l':
        flag->l = 1;
        break;
      case 'n':
        flag->n = 1;
        break;
      case 'c':
        flag->c = 1;
        break;
      case 'h':
        flag->h = 1;
        break;
      case 's':
        flag->s = 1;
        break;
      case 'f':
        flag->f = 1;
        pattern_file(optarg, pattern, flag->s);
        break;
      case 'o':
        flag->o = 1;
        break;
      case '?':
        fprintf(stderr, "not found flag %c\n", optopt);
        fprintf(stderr, "usage: [-e:ivclnhsf:o] [pattern] [file ...]\n");
        break;
    }
  }

  if (!flag->e && optind < argc && strlen(pattern) == 0) {
    add_pattern(pattern, argv[optind]);
    optind++;
  }
}

void flag_o(regex_t *regex, char *line) {
  regmatch_t match;
  int point = 0;
  while (regexec(regex, line + point, 1, &match, 0) == 0) {
    for (int i = match.rm_so; i < match.rm_eo; i++) {
      putchar(line[point + i]);
    }
    putchar('\n');

    point += match.rm_eo;
  }
}

FILE *open_file(char *name, int flag) {
  FILE *file = fopen(name, "r");
  if (file == NULL && !flag) {
    fprintf(stderr, "File not found\n");
  }
  return file;
}

int proccess(regex_t *regex, regmatch_t *match, char *line, flags flag,
             char *name, int multiple_files, int count_line) {
  int reg_error = regexec(regex, line, 1, match, 0);
  int match_f = (!reg_error);

  if (flag.v) {
    match_f = !match_f;
  }
  if (match_f) {
    if (flag.l) {
      if (!flag.c) {
        printf("%s\n", name);
      }
      return match_f;
    }
    if (!flag.c) {
      if (multiple_files && !flag.h) {
        printf("%s:", name);
      }
      if (flag.n) {
        printf("%d:", count_line);
      }
      if (flag.o) {
        flag_o(regex, line);
      } else {
        printf("%s\n", line);
      }
    }
  }
  return match_f;
}

void print_file(char *name, char *pattern, flags flag, int multiple_files) {
  FILE *file = open_file(name, flag.s);
  int reg_error = 0;
  int count = 0;
  int count_line = 0;
  int flag_break = 1;
  regex_t regex;
  regmatch_t match;
  char line[2048];

  reg_error = regcomp(&regex, pattern, REG_EXTENDED | (flag.i ? REG_ICASE : 0));
  if (reg_error != 0) {
    if (!flag.s) {
      fprintf(stderr, "couldn't compile regex: %s\n", pattern);
    }
    fclose(file);
    return;
  }
  while (flag_break && fgets(line, sizeof(line), file)) {
    count_line++;
    line[strcspn(line, "\n")] = '\0'; 
    int match_found =
        proccess(&regex, &match, line, flag, name, multiple_files, count_line);
    if (match_found) {
      count++;
      if (flag.l) {
        flag_break = 0;
      }
    }
  }
  if (flag.c) {
    if (multiple_files && !flag.h) {
      printf("%s:", name);
    }
    if (flag.l && count > 1) {
      count = 1;
    }
    printf("%d\n", count);
  }
  if (flag.l && flag.c && count > 0) {
    printf("%s\n", name);
  }

  regfree(&regex);
  fclose(file);
}
