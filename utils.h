#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdbool.h>
typedef union
{
  int i;
  float f;
  char *s;
  char c;
} answer_t;
typedef bool(*check_func)(char *);
typedef answer_t(*convert_func)(char *);
extern char *strdup(const char*);

int read_string(char *buf, int buf_siz);
bool is_number(char *str);
int ask_question_int(char *question);
char *ask_question_string(char *question);
answer_t ask_question(char *question, check_func check, convert_func convert);

#endif
