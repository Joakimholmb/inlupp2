#include "utils.h"
#include "linked_list.h"
#include "hash_table.h"
#include "webstore.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

struct table
{
  ioopm_hash_table_t *name_to_information;
  ioopm_hash_table_t *storage_to_name;
};

struct item
{
  char* name;
  char* desc;
  int price;
  unsigned int stock;
  char** location;
  bool in_stock;
};

struct cart
{
  item_t *list;
};

struct cart_list
{
  ioopm_list_t *shopping_carts;
};
/*
struct s
{
  item* locations;
  
};
*/

// *********** FUNC DECLARATIONS ************
static int cmpstringp(const void *p1, const void *p2);
void sort_keys(char *keys[], size_t no_keys);
item_t *add_merch();
void list_merch(ioopm_hash_table_t *ht);
static void list_merch_aux(ioopm_list_t *list, size_t num_elements,  int i);
void edit_merch(ioopm_hash_table_t *ht, item_t *item);
void remove_merch(ioopm_hash_table_t *ht, item_t *item);
void print_menu();
char ask_question_menu();
void event_loop();
char *void_to_char(ioopm_elem_t arg);
int ioopm_string_knr_hash(ioopm_elem_t str);


// ********** FUNCTIONS ***********

static int cmpstringp(const void *p1, const void *p2)
{
  return strcmp(* (char * const *) p1, * (char * const *) p2);
}

void sort_keys(char *keys[], size_t no_keys)
{
  qsort(keys, no_keys, sizeof(char *), cmpstringp);
}

//TODO: Lägg till check att merch inte finns tidigare
item_t *add_merch(void)
{
  //ioopm_hash_table_insert(ht, merch->name, merch)
  item_t *merch = calloc(1, sizeof(item_t));
  //item_t merch;
  char *answer_name = ask_question_string("Önskat namn:\n");
  merch->name = answer_name;
  char *answer_desc = ask_question_string("Önskat description:\n");
  merch->desc = answer_desc;
  int answer_price = ask_question_int("önskat prisiis\n");
  merch->price = answer_price;
  merch->in_stock = false;

  

  return merch;
}

void list_merch(ioopm_hash_table_t *ht)
{
  ioopm_list_t *list = ioopm_hash_table_keys(ht);
  size_t num_elements = ioopm_linked_list_size(list);
  int i = 0;
  
  list_merch_aux(list, num_elements, i);

  while(true)
    {
      char *answer = ask_question_string("Mer än 20?\n");

      if(strcmp("Ja", answer))
        {
          i = i + 20;
          list_merch_aux(list, num_elements, i);
        }
      else if(strcmp("Nej", answer))
        {
          return;
        }
    }
}

static void list_merch_aux(ioopm_list_t *list, size_t num_elements, int i)
{
   int j = i;
   char **buf = calloc(num_elements, sizeof(char*));
   while(i < num_elements && (i % 20 != 0 || i == 0))
    {
      item_t *ptr_key =  ioopm_linked_list_get(list, i).ptr_value;
      buf[i] = ptr_key->name;
      ++i;
    }
  //sort_keys(buf, num_elements);
  
  while(j < i)
    {
      printf("%s\n", buf[j]);
      ++j;
    }

  return;
}

void edit_merch(ioopm_hash_table_t *ht, item_t *item)
{
  while(true)
    {
      char *answer = ask_question_string("Vad vill du ändra på?\n");
      if(strcmp("Name", answer))
        {
          char *new_name = ask_question_string("Nya namnet:\n");
          ioopm_elem_t new_alias = { .ptr_value = new_name};
          if(!ioopm_hash_table_has_key(ht, new_alias))
            {
              ioopm_elem_t old_item = { .ptr_value = item};
              item->name = new_name;
              ioopm_elem_t key = { .ptr_value = new_name};
              ioopm_elem_t new_item = { .ptr_value = item};
              ioopm_hash_table_insert(ht, key, new_item);
              ioopm_hash_table_remove(ht, old_item);
            }
          else
            {
              printf("Has name already\n");
            }
        }
      else if(strcmp("Description", answer))
        {
          char *new_desc = ask_question_string("Nya info:\n");
          item->desc = new_desc;
        }
      else if(strcmp("Price", answer))
        {
          int new_price = ask_question_int("Nya priset:\n");
          item->price = new_price;
        }
      else if(strcmp("Exit", answer))
        {
          return;
        }
    }
}

void remove_merch(ioopm_hash_table_t *ht, item_t *item)
{
  char *answer = ask_question_string("Vilke vara vill du ta bort?");
  ioopm_elem_t to_remove = { .ptr_value = answer};
  ioopm_hash_table_remove(ht, to_remove);
}

void print_menu()
{
  printf("[L]ägga till en vara\n\
[T]a bort en vara\n\
[R]edigera en vara\n\
Ån[g]ra senaste ändringen\n\
Lista[h]ela varukatalogen\n\
[A]vsluta\n");
}

char ask_question_menu()
{
  char *ch;

  do
    {
      print_menu();
      ch = ask_question_string("Vad önskas? ");
    }
  while(strlen(ch) != 1 || (*ch != 'L' && *ch != 'l' &&
                           *ch != 'T' && *ch != 't' &&
                           *ch != 'R' && *ch != 'r' &&
                           *ch != 'G' && *ch != 'g' &&
                           *ch != 'H' && *ch != 'h' &&
                           *ch != 'A' && *ch != 'a'));

  return toupper(*ch);
  
}


void event_loop(void)
{
  table_t *tables = calloc(1, sizeof(table_t));
  tables->name_to_information = ioopm_hash_table_create(ioopm_string_knr_hash, NULL, NULL);
  char choice;
  
  do
    {
      choice = ask_question_menu();

      if(choice=='L')
        {
         item_t *new_merch = add_merch();
         ioopm_elem_t new_merch_name = {.ptr_value = new_merch->name};
         ioopm_elem_t new_merch_item = {.ptr_value = new_merch};
         ioopm_hash_table_insert(tables->name_to_information, new_merch_item, new_merch_item);
         
        }
      else if(choice=='T')
        {
         
        }
      else if(choice=='R')
        {
          
        }
      else if(choice=='G')
        {
          printf("Not yet implemented!");
        }
      else if(choice=='H')
        {
          list_merch(tables->name_to_information);
        }
      else
        {
          printf("Ends program");
        }
    }
  while(choice != 'A');
}


char *void_to_char(ioopm_elem_t arg)
{
  char *ptr_char = arg.ptr_value;

  return ptr_char;
}

int ioopm_string_knr_hash(ioopm_elem_t str)
{
  int result = 0;
  int ascii = 32;
  char *s = str.ptr_value;
  
  do
    {
      result = result * ascii + *s;
    }
  while (*++s != '\0');
  return result;
}

int main (void)
{
  event_loop();

  return 0;
}
