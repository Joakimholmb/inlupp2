/*
  Albin Antti & Nils Hedberg
  IT2B
  IOOPM
  September/October 2018
*/

#pragma once
#include "linked_list.h"
#include <stdbool.h>
#include <stdlib.h>

// Represents an element inside a linked list
struct link
{
  ioopm_elem_t elem;
  struct link *next;
};

// Represents a linked list
struct list
{
  ioopm_link_t *first;
  ioopm_link_t *last;
  size_t size;
  ioopm_eq_function eq_func;
};

ioopm_link_t *link_create(ioopm_elem_t value, ioopm_link_t *next);
