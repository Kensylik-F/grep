#ifndef S21_GREP
#define S21_GREP

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  int e;  
  int i;  
  int v;  
  int c;  
  int l;  
  int n;  

  int h;  
  int s;  
  int f;  
  int o;  
} flags;

void add_pattern(char *pattern, char *tmpl);
void pattern_file(char *name, char *pattern, int flag);
void parse_flag(int argc, char **argv, flags *flag, char *pattern);
void flag_o(regex_t *regex, char *line);
FILE *open_file(char *name, int flag);
int proccess(regex_t *regex, regmatch_t *match, char *line, flags flag,
             char *name, int multiple_files, int count_line);
void print_file(char *name, char *pattern, flags flag, int multiple_files);

#endif