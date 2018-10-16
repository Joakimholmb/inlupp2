/*
  Albin Antti & Nils Hedberg
  IT2B
  IOOPM
  September/October 2018
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "hash_table.h"
#include "linked_list.h"
#include "list_iterator_private.h"
#include "iterator.h"

#define Initial_buckets 17
#define load_factor 0.75

struct entry
{
  ioopm_elem_t key;
  ioopm_elem_t value;
  ioopm_entry_t *next;
};

struct hash_table
{
  size_t no_buckets;
  ioopm_entry_t **buckets;
  ioopm_hash_function hash_function;
  ioopm_eq_function key_eq_function;
  ioopm_eq_function value_eq_function;
  size_t size;
};

//finds the entry before the given key in the list
static ioopm_entry_t *find_previous_entry_for_key(ioopm_hash_function hf, ioopm_entry_t *entry, int key)
{
  if(entry -> next == NULL || hf(entry -> next -> key) == key)
    {
      return entry;
    }
  else
    {
      return find_previous_entry_for_key(hf, entry -> next, key);
    }
}

//creates a new entry with a given key, value and next pointer
static ioopm_entry_t *entry_create(ioopm_elem_t key, ioopm_elem_t value, ioopm_entry_t *next)
{
  ioopm_entry_t *entry  = calloc(1, sizeof(ioopm_entry_t));
  entry -> value = value;
  entry -> key = key;
  entry -> next = next;
  return entry;
}

size_t ioopm_hash_table_capacity(ioopm_hash_table_t *ht)
{
  return ht -> no_buckets;
}

// Destroys the following entry, returns its value, reforms the linked list
static ioopm_elem_t destroy_next_entry(ioopm_entry_t *prev_entry)
{
  ioopm_entry_t *to_remove = prev_entry ->next;
  prev_entry -> next = to_remove -> next;
  ioopm_elem_t removed_value = to_remove -> value;
  free(to_remove);
  return removed_value;
}

static size_t select_prime(size_t current_prime)
{
  size_t primes[] = {17, 31, 67, 127, 257, 509, 1021, 2053, 4099, 8191, 16381, 32771, 65537, 131101};
  for(size_t i = 0; i < sizeof(primes); i++)
    {
      if(primes[i] > current_prime)
        return primes[i];
    }
  return (size_t) 16381;
}

static ioopm_hash_table_t *ioopm_hash_table_create_aux(ioopm_hash_table_t *ht, size_t no_buckets)
{
  ioopm_entry_t **buckets = calloc(no_buckets, sizeof(ioopm_entry_t *));
  ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
  result -> buckets = buckets;
  result -> size = 0;
  result -> no_buckets = no_buckets;
  result -> hash_function = ht -> hash_function;
  result -> key_eq_function = ht -> key_eq_function;
  result -> value_eq_function = ht -> value_eq_function;
  for (size_t i = 0; i < no_buckets; i++)
    {
      result -> buckets[i] = entry_create((ioopm_elem_t) {0}, (ioopm_elem_t){0}, NULL);
    }
  return result;
}

static ioopm_hash_table_t *resize_hash_table(ioopm_hash_table_t *ht)
{
  size_t size = ht -> size;
  size_t no_buckets = ht -> no_buckets;
  long load = ((long) size / (long) no_buckets);
  if(load >= load_factor && (long) select_prime(no_buckets) > no_buckets)
    {
      ioopm_list_t *keys = ioopm_hash_table_keys(ht);
      ioopm_list_t *values = ioopm_hash_table_values(ht);
      ioopm_hash_table_t *new_ht = ioopm_hash_table_create_aux(ht, select_prime(ht->no_buckets));
      for(size_t i = 0; i < ioopm_linked_list_size(keys); i++)
        {
          ioopm_hash_table_insert(new_ht, ioopm_linked_list_get(keys, i), ioopm_linked_list_get(values, i));
        }
      ioopm_linked_list_destroy(keys);
      ioopm_linked_list_destroy(values);
      
      // Clear the original ht
      ioopm_hash_table_clear(ht);
      
      // Clear the dummy elements
      for(size_t i = 0; i < ht -> no_buckets;i++)
        {
          free(ht -> buckets[i]);
        }
      free(ht -> buckets);

      // Assign ht to the new hash table buckets
      ht -> buckets = new_ht -> buckets;
      ht -> no_buckets = new_ht -> no_buckets;
      ht -> size = new_ht  -> size;

      // Free memory storing the new ht
      free(new_ht);
    }
  return ht;
}


void ioopm_hash_table_insert(ioopm_hash_table_t *ht, ioopm_elem_t key, ioopm_elem_t value)
{
  size_t key_hash = ht->hash_function(key);
  size_t bucket = key_hash % ht -> no_buckets;
  ioopm_entry_t *previous = find_previous_entry_for_key(ht->hash_function, ht->buckets[bucket], key_hash);
  ioopm_entry_t *current = previous -> next;
  if (current != NULL && ht->hash_function(current->key) == key_hash)
    {
      current -> value = value;
    }
  else
    {
      previous -> next = entry_create(key, value, current);
      ht->size += 1;
    }
  ht = resize_hash_table(ht);
}

char *ioopm_strdup(char *str)
{
  size_t len = strlen(str);
  char *result = calloc(len + 1, sizeof(char));
  strncpy(result, str, len);
  return result;
}

ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function hf, ioopm_eq_function kef, ioopm_eq_function vef)
{
  ioopm_entry_t **buckets = calloc(Initial_buckets, sizeof(ioopm_entry_t *));
  ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
  result -> buckets = buckets;
  result -> no_buckets = Initial_buckets;
  result -> size = 0;
  result -> hash_function = hf;
  result -> key_eq_function = kef;
  result -> value_eq_function = vef;
  for (size_t i = 0; i < Initial_buckets; i++)
    {
      result -> buckets[i] = entry_create((ioopm_elem_t) {0}, (ioopm_elem_t){0}, NULL);
    }
  return result;
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht)
{
  ioopm_hash_table_clear(ht);
  for(size_t i = 0; i < ht -> no_buckets; ++i)
    {
      free(ht -> buckets[i]);
    }
  free(ht -> buckets);
  free(ht);
}

ioopm_elem_t *ioopm_hash_table_lookup(ioopm_hash_table_t *ht, ioopm_elem_t key)
{
  size_t key_hash = ht->hash_function(key);
  size_t bucket = key_hash % ht -> no_buckets;
  ioopm_entry_t *entry = find_previous_entry_for_key(ht -> hash_function, ht -> buckets[bucket], key_hash);
  if(entry ->next && ht->hash_function(entry->next->key) == (int) key_hash)
    {
      return &entry -> next -> value;
    }
  else
    {
      return NULL;
    }
}

ioopm_elem_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, ioopm_elem_t key)
{
  size_t key_hash = ht->hash_function(key);
  size_t bucket = key_hash % ht -> no_buckets;
  ioopm_entry_t *prev_entry = find_previous_entry_for_key(ht->hash_function, ht->buckets[bucket], key_hash);
  ht->size -= 1;
  return destroy_next_entry(prev_entry);
}

size_t ioopm_hash_table_size(ioopm_hash_table_t *ht)
{
  return ht->size;
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht)
{
  return ht->size == 0;
}

void ioopm_hash_table_clear(ioopm_hash_table_t *ht)
{
  for(size_t i = 0; i < ht -> no_buckets;++i)
    {
      ioopm_entry_t *dummy = ht->buckets[i];
      while(dummy -> next != NULL)
        {
          destroy_next_entry(dummy);
        }
    }
  ht -> size = 0;
}

static void accumulate_keys(ioopm_entry_t *cursor, ioopm_list_t *key_list, size_t *index)
{
  if(cursor)
    {
      ioopm_linked_list_insert(key_list, *index, cursor -> key);
      *index += 1;
      accumulate_keys(cursor->next, key_list, index);
    }
}

static void accumulate_values(ioopm_entry_t *cursor, ioopm_list_t *val_list, size_t *index)
{
  if(cursor)
    {
      ioopm_linked_list_insert(val_list, *index, cursor -> value);
      *index += 1;
      accumulate_values(cursor->next, val_list, index);
    }
}

ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht)
{
  ioopm_list_t *key_list = ioopm_linked_list_create(ht -> key_eq_function);
  size_t index = 0;
  for(size_t i = 0; i < ht -> no_buckets;++i)
    {
      ioopm_entry_t *dummy = ht->buckets[i];
      accumulate_keys(dummy->next, key_list, &index);
    }
  return key_list;
}

ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht)
{
  ioopm_list_t *val_list = ioopm_linked_list_create(ht -> value_eq_function);
  size_t index = 0;
  for(size_t i = 0; i < ht -> no_buckets;++i)
    {
      ioopm_entry_t *dummy = ht->buckets[i];
      accumulate_values(dummy->next, val_list, &index);
    }
  return val_list;
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, ioopm_elem_t key)
{
  return ioopm_hash_table_lookup(ht, key) != NULL;
}

bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, ioopm_elem_t value)
{
  ioopm_list_t *values = ioopm_hash_table_values(ht);
  ioopm_list_iterator_t *iter = ioopm_list_iterator(values);
  while(ioopm_iterator_has_next(iter))
    {
      if(ht->value_eq_function(value, ioopm_iterator_current(iter)))
        {
          ioopm_linked_list_destroy(values);
          ioopm_iterator_destroy(iter);
          return true;
        } 
    }
  ioopm_linked_list_destroy(values);
  ioopm_iterator_destroy(iter);
  return false;
}

bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate P, ioopm_elem_t arg)
{
  size_t size = ioopm_hash_table_size(ht);
  ioopm_list_t *keys = ioopm_hash_table_keys(ht);
  ioopm_list_iterator_t *key_iter = ioopm_list_iterator(keys);
  ioopm_list_t *values = ioopm_hash_table_values(ht);
  ioopm_list_iterator_t *value_iter = ioopm_list_iterator(values);
  
  for (size_t i = 0; i < size; ++i)
    {
      if(!P(ioopm_iterator_current(key_iter), ioopm_iterator_current(value_iter), arg))
        {
          ioopm_iterator_destroy(key_iter);
          ioopm_linked_list_destroy(keys);
          ioopm_iterator_destroy(value_iter);
          ioopm_linked_list_destroy(values);
          return false; 
        }
      ioopm_iterator_next(key_iter);
      ioopm_iterator_next(value_iter);
    }
  ioopm_iterator_destroy(key_iter);
  ioopm_linked_list_destroy(keys);
  ioopm_iterator_destroy(value_iter);
  ioopm_linked_list_destroy(values);
  return true;
}

bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate P, ioopm_elem_t arg)
{
  size_t size = ioopm_hash_table_size(ht);
  ioopm_list_t *keys = ioopm_hash_table_keys(ht);
  ioopm_list_iterator_t *key_iter = ioopm_list_iterator(keys);
  ioopm_list_t *values = ioopm_hash_table_values(ht);
  ioopm_list_iterator_t *value_iter = ioopm_list_iterator(values);
  
  for (size_t i = 0; i < size; ++i)
    {
      if(P(ioopm_iterator_current(key_iter), ioopm_iterator_current(value_iter), arg))
        {
          ioopm_iterator_destroy(key_iter);
          ioopm_linked_list_destroy(keys);
          ioopm_iterator_destroy(value_iter);
          ioopm_linked_list_destroy(values);
          return true; 
        }
      ioopm_iterator_next(key_iter);
      ioopm_iterator_next(value_iter);
    }
  ioopm_iterator_destroy(key_iter);
  ioopm_linked_list_destroy(keys);
  ioopm_iterator_destroy(value_iter);
  ioopm_linked_list_destroy(values);
  return false;
}

void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function F, ioopm_elem_t arg)
{
  for(size_t i = 0; i < ht -> no_buckets;++i)
    {
      ioopm_entry_t *cursor = ht -> buckets[i] -> next;
      while(cursor)
        {
          F(cursor -> key, &cursor -> value, arg);
          cursor = cursor -> next;
        }
    } 
}
