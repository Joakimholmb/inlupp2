/*
  Albin Antti & Nils Hedberg
  IT2B
  IOOPM
  September/October 2018
*/

#include <stdbool.h>
#include "list_iterator_private.h"
#include "iterator.h"
#include <stdlib.h>

struct iterator
{
  ioopm_link_t *cursor;
  ioopm_link_t *sentinel;
  ioopm_list_t *list;
};

ioopm_list_iterator_t *ioopm_list_iterator(ioopm_list_t *list)
{
  ioopm_list_iterator_t *iter = calloc(1, sizeof(ioopm_list_iterator_t));
  iter -> cursor = link_create((ioopm_elem_t) {0}, list -> first);
  iter -> sentinel = iter -> cursor;
  iter -> list = list;
  return iter;
}

bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter)
{
  if(iter->cursor->next) return true;
  return false;
}

ioopm_elem_t ioopm_iterator_next(ioopm_list_iterator_t *iter)
{
  iter -> cursor = iter -> cursor -> next;
  return iter -> cursor -> elem;
}

ioopm_elem_t ioopm_iterator_remove(ioopm_list_iterator_t *iter)
{
  ioopm_elem_t rm_value = iter -> cursor -> next -> elem;
  ioopm_link_t *to_remove = iter -> cursor -> next;
  if(iter -> cursor -> next == iter -> list -> first)
    iter -> list -> first = iter -> list -> first -> next;
  iter -> cursor -> next = iter -> cursor -> next -> next;
  if(!ioopm_iterator_has_next(iter))
    iter -> list -> last = iter -> cursor;
  free(to_remove);
  iter -> list -> size -= 1;
  return rm_value;
}

void ioopm_iterator_insert(ioopm_list_iterator_t *iter, ioopm_elem_t elem)
{
  iter -> cursor -> next = link_create(elem, iter -> cursor -> next);
  if(iter -> list -> first == iter -> cursor -> next -> next)
    iter -> list -> first = iter -> cursor -> next;
  if(iter -> list -> last == iter -> cursor)
    iter -> list -> last = iter -> cursor -> next;
  iter -> list -> size += 1;
}

void ioopm_iterator_reset(ioopm_list_iterator_t *iter)
{
  iter -> cursor = link_create((ioopm_elem_t) {0}, iter -> list -> first);
  free(iter -> sentinel);
  iter -> sentinel = iter -> cursor;
}

ioopm_elem_t ioopm_iterator_current(ioopm_list_iterator_t *iter)
{
  return iter -> cursor -> next -> elem;
}

void ioopm_iterator_destroy(ioopm_list_iterator_t *iter)
{
  free(iter -> sentinel);
  free(iter);
}
