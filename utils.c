#include "utils.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>

bool not_empty(char *str)
{
  return strlen(str) > 0;
}

answer_t ask_question(char *question, check_func check, convert_func convert)
{
  int buf_siz = 255;
  char buf[buf_siz];
  answer_t answer;
  do
    {
      printf("%s\n", question);
      read_string(buf, buf_siz);        
    }
  while (!(check(buf)));
  answer = convert(buf);
  return answer; 
}

char *ask_question_string(char *question)
{
  answer_t answer = ask_question(question, not_empty, (convert_func) strdup);
  return answer.s;
}

int ask_question_int (char *question)
{
  answer_t answer = ask_question(question, is_number, (convert_func) atoi);
  return answer.i;
}

bool is_number(char *str)
{
  int str_length = strlen(str);
  for (int i = 0; i < str_length; ++i)
    {
      char element = (str)[i];
      if  (!(isdigit(element) || (element == '-' && i == 0 && str_length > 1)))
        {
          return false;
        }      
    }
  return str_length > 0;
}

int read_string(char *buf, int buf_siz)
{
  int i = 0;
  do
    {
      int c = getchar();
      if (c != '\n' && c != EOF)
        {
          buf[i] = c;
          i = i + 1;
        }
      else
        {
          buf[i] = '\0';
          return i;
        }
    }
  while (i <= (buf_siz - 1));
  int k;
  do
    {
      k = getchar();
    }
  while (k != '\n' && k != EOF);
  buf[i] = '\0';
  return i;
}
/* 
int main(void)
{
  //int buf_siz = 255;
  char *question = "hur gammal är du?";
  char *question2 = "vad heter du?";
  char *str = ask_question_string2(question2);
  printf("Hej %s!\n", str);
  int str2 = ask_question_int2(question);
  printf("Du är %d gammal!\n", str2);
  return 0;
}
*/

