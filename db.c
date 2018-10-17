#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define ARRSIZE 16

typedef struct item
{
  char *name;
  char *desc;
  int price;
  char *store;
  int amount;
} item_t; //typedef struct item item_t;

bool is_shelf(char *str);
char *ask_question_shelf(char *question);
void print_item(item_t *item);
item_t make_item(char *n, char *d, int p, char *s, int a);
item_t input_item();
char *magick(char **ar1, char **ar2, char **ar3, int r);
void list_db(item_t *items, int no_items);
void edit_db(item_t *items, int no_items);
void print_menu();
char ask_question_menu();
int add_item_to_db(item_t *items, int no_items);
int remove_item_from_db(item_t *items, int no_items);
void event_loop(item_t *items, int db_siz);


//********************* FUNCTIONS ******************


int main(int argc, char *argv[])
{
  char *array1[] = { "Laser",        "Polka",    "Extra" };
  char *array2[] = { "förnicklad",   "smakande", "ordinär" };
  char *array3[] = { "skruvdragare", "kola",     "uppgift" };

  if (argc < 2)
  {
    printf("Usage: %s number\n", argv[0]);
  }
  else
  {
    item_t db[16]; // Array med plats för 16 varor
    int db_siz = 0; // Antalet varor i arrayen just nu

    int items = atoi(argv[1]); // Antalet varor som skall skapas

    if (items > 0 && items <= 16)
    {
      for (int i = 0; i < items; ++i)
      {
        // Läs in en vara, lägg till den i arrayen, öka storleksräknaren
        item_t item = input_item();
        db[db_siz] = item;
        ++db_siz;
      }
    }
    else
    {
      puts("Sorry, must have [1-16] items in database.");
      return 1; // Avslutar programmet!
    }

    for (int i = db_siz; i < 16; ++i)
      {
        char *name = magick(array1, array2, array3, 3); // TODO: Lägg till storlek
        char *desc = magick(array1, array2, array3, 3); // TODO: Lägg till storlek
        int price = random() % 200000;
        char shelf[] = { random() % ('Z'-'A') + 'A',
                         random() % 10 + '0',
                         random() % 10 + '0',
                         '\0' };
        int amount = random() % 100;
        item_t item = make_item(name, desc, price, shelf, amount);

        db[db_siz] = item;
        ++db_siz;
      }
    list_db(db, db_siz);
    printf("\n");
    event_loop(db, db_siz);
  }
  return 0;
}

/*
int main()
{
  //char ch = ask_question_menu();
  //printf("%c\n", ch);

  return 0;
}
*/

bool is_shelf(char *str)
{
  int length = strlen(str);
  //printf("%d\n", length);

  if(length<2)
    {
      return false;
    }

  if(isalpha(str[0]))
    {
      if(is_number(++str))
        {
          return true;
        }
      else
        {
          return false;
        }
    }
  else
    {
      return false;
    }
}

char *ask_question_shelf(char *question)
{
  return ask_question(question, is_shelf, (convert_func) strdup).s;
}

void print_item(item_t *item)
{
  printf("Name: %s\n", item->name);
  printf("Desc: %s\n", item->desc);
  printf("Price: %d\n", (item->price/100));
  printf("Shelf: %s\n", item->store);
  printf("Amount: %d\n", item->amount);
}

item_t make_item(char *n, char *d, int p, char *s, int a)
{
  item_t x = {.name = n, .desc = d, .price = p, .store = s, .amount = a};

  return x;
}

item_t input_item()
{
  char *name = ask_question_string("Skriv in namn på varan: ");
  char *desc = ask_question_string("Skriv in beskrivning på varan: ");
  int price = ask_question_int("Skriv in priset på varan: ");
  char *shelf = ask_question_shelf("Skriv in hyllan på varan: ");
  int amount = ask_question_int ("Skriv in antal av varan: ");

  return make_item(name, desc, price, shelf, amount);
}

char *magick(char **ar1, char **ar2, char **ar3, int r)
{
  char buf[255];
  int i = 0;
  
  int rand1 = random() % r;
  int rand2 = random() % r;
  int rand3 = random() % r;

  char *x = ar1[rand1];
  char *y = ar2[rand2];
  char *z = ar3[rand3];

  for(char *cursor = x; *cursor != '\0'; cursor++)
    {
      buf[i] = *cursor;
      ++i;
    }
  buf[i] = '-';
  ++i;
  for(char *cursor = y; *cursor != '\0'; cursor++)
    {
      buf[i] = *cursor;
      ++i;
    }
  buf[i] = '-';
  ++i;
  for(char *cursor = z; *cursor != '\0'; cursor++)
    {
      buf[i] = *cursor;
      ++i;
    }
  buf[i] = '\0';
  
  return (strdup(buf));
}

void list_db(item_t *items, int no_items)
{
  int i;
  for(i=0; i<no_items; i++)
    {
      printf("%d. %s\n", i+1, items[i].name);
    }
}

void edit_db(item_t *items, int no_items)
{
  int numedit;
  do
    {
      numedit = ask_question_int("Vilken vara vill du ändra på? ");
    }
  while(numedit<1 && numedit>no_items); // ABSTRACTION LEAK

  numedit-=1;
  
  print_item(&items[numedit]);

  item_t newitem = input_item();
  items[numedit] = newitem;
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

int add_item_to_db(item_t *items, int no_items)
{
  if(no_items == ARRSIZE)
    {
      printf("WARNING! Storage limit = 16!\n");
      return no_items;
    }
  else
    {
      items[no_items] = input_item();
    }
  return no_items+1;
}

int remove_item_from_db(item_t *items, int no_items)
{
  int numedit = 0;
  
  list_db(items, no_items);

  do
    {
      numedit = ask_question_int("Vilken vara vill du ta bort?");
    }
  while(numedit<= 0 || numedit>no_items+1);

  --numedit; // readjust array index

  /*
  if(numedit == no_items-1)
    {
      return no_items-1;
    }
  else
    {
      for(; numedit < no_items-1 && numedit <= ARRSIZE; ++numedit)
        {
          items[numedit] = items[numedit+1];
        }
    }
  */

  for(; numedit < no_items-1 && numedit <= ARRSIZE; ++numedit)
    {
      items[numedit] = items[numedit+1];
    }
  
  return no_items-1;
}

void event_loop(item_t *items, int db_siz)
{

  char choice;
  
  do
    {
      choice = ask_question_menu();

      if(choice=='L')
        {
          db_siz = add_item_to_db(items, db_siz);
        }
      else if(choice=='T')
        {
          db_siz = remove_item_from_db(items, db_siz);
        }
      else if(choice=='R')
        {
          edit_db(items, db_siz);
        }
      else if(choice=='G')
        {
          println("Not yet implemented!");
        }
      else if(choice=='H')
        {
          list_db(items, db_siz);
        }
      else
        {
          println("Ends program");
        }
    }
  while(choice != 'A');
  
  /*
  do
    {
      choice = ask_question_menu();
      
      switch(choice)
        {
        case 'L':
          {
            db_siz = add_item_to_db(items, db_siz);
          }
        case 'T':
          {
            db_siz = remove_item_from_db(items, db_siz);
          }
        case 'R':
            {
              edit_db(items, db_siz);
            }
        case 'G':
            {
              println("Not yet implemented!");
            }
        case 'H':
            {
              list_db(items, db_siz);
            }
        case 'A':
            {
              println("Ends program");
            }          
        }     
    }
  while(choice != 'A');
  */
}
