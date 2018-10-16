/*
  Albin Antti & Nils Hedberg
  IT2B
  IOOPM
  September/October 2018
*/

#include "linked_list.h"
#include "list_iterator_private.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>

// Creates a link with a value and a next pointer to a following link
ioopm_link_t *link_create(ioopm_elem_t value, ioopm_link_t *next)
{
  ioopm_link_t *link = calloc(1, sizeof(ioopm_link_t));

  link -> elem = value;
  link -> next = next;

  return link;
}

// Initializes a linked list
ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function eq_func)
{
  ioopm_list_t *l = calloc(1, sizeof(ioopm_list_t));
  l -> first = NULL;
  l -> last = NULL;
  l -> size = 0;
  l -> eq_func = eq_func;
  return l;
}

size_t ioopm_linked_list_size(ioopm_list_t *list)
{
  return list -> size;
}

bool ioopm_linked_list_is_empty(ioopm_list_t *list)
{
  return list -> size == 0;
}

static ioopm_elem_t destroy_link(ioopm_link_t *link)
{
  ioopm_elem_t rm_value = link -> elem;
  free(link);
  return rm_value;
}

ioopm_elem_t ioopm_linked_list_remove(ioopm_list_t *list, size_t index)
{
  ioopm_link_t *cursor = list->first;
  
  if(index == 0)
    {
      list -> first = cursor -> next;
      if(!list->first) list -> last = NULL;
      list -> size -= 1;
      return destroy_link(cursor);
    }
  
  for(size_t i = 0; i < index-1; ++i)
    {
      cursor = cursor ->next; 
    }
  
  if(!cursor -> next -> next)
    {
      list -> last = cursor;
    }
  
  ioopm_link_t *to_remove = cursor -> next;
  cursor -> next = to_remove -> next;
  list -> size -= 1;
  return destroy_link(to_remove);
  
}

ioopm_elem_t ioopm_linked_list_get(ioopm_list_t *list, size_t index)
{
  ioopm_link_t *cursor = list->first;
  for(size_t i = 0; i < index; ++i)
    {
      cursor = cursor ->next; 
    }

  return cursor->elem;
}

void ioopm_linked_list_prepend(ioopm_list_t *list, ioopm_elem_t value)
{
  list -> first = link_create(value, list -> first);
  if(list -> size == 0)
    list -> last = list -> first;
  list -> size += 1;
}

void ioopm_linked_list_append(ioopm_list_t *list, ioopm_elem_t value)
{
  if(list -> last)
    {
      list -> last -> next = link_create(value, NULL);  
      list -> last = list -> last -> next;
      list -> size += 1;
    }
  else
    ioopm_linked_list_prepend(list, value);
}

void ioopm_linked_list_insert(ioopm_list_t *list, int index, ioopm_elem_t value)
{
  ioopm_link_t *cursor = list -> first;
  // Iterate through the list to find link corresponding to index
  for(int i = 0; i < index-1;++i)
    {
      cursor = cursor -> next;
    }
  // If the first element is being changed, prepend the value into the list
  if(index == 0)
    {
      ioopm_linked_list_prepend(list, value);
    }
  // If the last element is being changed, append the value
  else if(!cursor->next)
    {
      ioopm_linked_list_append(list, value);
    }
  // If the current index is occupied, extend the list and place the value on index
  else
    {
      cursor -> next = link_create(value, cursor -> next);
      list -> size += 1;
    }
}

bool ioopm_linked_list_contains(ioopm_list_t *list, ioopm_elem_t elem)
{
  ioopm_link_t *cursor = list -> first;
  while(cursor)
    {
      if(list -> eq_func(cursor -> elem, elem))
        return true;
      cursor = cursor -> next;
    }
  return false;
}

static void destroy_backwards(ioopm_link_t *cursor)
{
  if(cursor)
    {
      destroy_backwards(cursor -> next);
      destroy_link(cursor);
    }
}

void ioopm_linked_list_clear(ioopm_list_t *list)
{
  destroy_backwards(list -> first);
  list -> first = NULL;
  list -> last = NULL;
  list -> size = 0;
}

void ioopm_linked_list_destroy(ioopm_list_t *list)
{
  ioopm_linked_list_clear(list);
  free(list);
}

bool ioopm_linked_list_all(ioopm_list_t *list, bool (*prop)(int, void *), void *x)
{
  if(list -> first == NULL)
    return false;
  
  ioopm_link_t *cursor = list -> first;
  for(size_t i = 0; i < list -> size; i++)
    {
      if(!prop(cursor->elem.int_value, x))
        return false;
      cursor = cursor -> next;
    }
  return true;
}

bool ioopm_linked_list_any(ioopm_list_t *list, bool (*prop)(int, void *), void *x)
{
  if(list -> first == NULL)
    return false;
  
  ioopm_link_t *cursor = list -> first;
  for(size_t i = 0; i < list -> size; i++)
    {
      if(prop(cursor->elem.int_value, x))
        return true;
      cursor = cursor -> next;
    }
  return false;
}

void ioopm_linked_apply_to_all(ioopm_list_t *list, void (*fun)(ioopm_elem_t *, ioopm_elem_t *), ioopm_elem_t *x)
{
  ioopm_link_t *cursor = list -> first;
  if(!cursor)
    return;
  for(size_t i = 0; i < list -> size; i++)
    {
      fun(&cursor -> elem, x);
      cursor = cursor -> next;
    }
}
