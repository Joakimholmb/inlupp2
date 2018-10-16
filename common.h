/*
  Albin Antti & Nils Hedberg
  IT2B
  IOOPM
  September/October 2018
*/

#pragma once

#include <stdbool.h>

union elem
{
  int int_value;
  float float_value;
  char char_value;
  void *ptr_value;
};

typedef union elem ioopm_elem_t;
typedef struct list ioopm_list_t;
typedef bool(*ioopm_eq_function)(ioopm_elem_t a, ioopm_elem_t b);
