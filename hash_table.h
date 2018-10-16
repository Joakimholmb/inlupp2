/*
  Albin Antti & Nils Hedberg
  IT2B
  IOOPM
  September/October 2018
*/

#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include "common.h"

typedef struct entry ioopm_entry_t;
typedef struct hash_table ioopm_hash_table_t;

typedef int (*ioopm_hash_function)(ioopm_elem_t key);

typedef bool (*ioopm_predicate)(ioopm_elem_t key, ioopm_elem_t value, ioopm_elem_t extra);
typedef void (*ioopm_apply_function)(ioopm_elem_t key, ioopm_elem_t *value, ioopm_elem_t extra);

/// @brief Create a new hash table
/// @param hf A hash function for your selected key type
/// @precondition The hash function hf must return positive integers. Negative integers will return in errors.
/// @param kef A equality test function for your chosen key type
/// @param vef A equality test function for your chosen value type
/// @return A new empty hash table
ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function hf, ioopm_eq_function kef, ioopm_eq_function vef);

/// @brief add key => value entry in hash table h
/// @param h hash table operated upon
/// @param key key to insert
/// @param value value to insert
void ioopm_hash_table_insert(ioopm_hash_table_t *h, ioopm_elem_t key, ioopm_elem_t value);

/// @brief returns the number of buckets in a given hash table h
/// @param h hash table operated upon
/// @returns the number of buckets in type size_t
size_t ioopm_hash_table_capacity(ioopm_hash_table_t *h);

/// @brief duplicates a string
/// @param str string to be duplicated
/// @returns a pointer to a new copy of the input string
char *ioopm_strdup(char *str);

/// @brief lookup value for key in hash table ht
/// @param h hash table operated upon
/// @param key key to lookup
/// @return a pointer to the elem_t mapped to by key.
///         If the returned pointer points to NULL, no value was found. 
ioopm_elem_t *ioopm_hash_table_lookup(ioopm_hash_table_t *h, ioopm_elem_t key);

/// @brief remove any mapping from key to a value
/// @param h hash table operated upon
/// @param key key to remove, only keys existing in the hash table are valid
/// @return the value mapped to by key 
ioopm_elem_t ioopm_hash_table_remove(ioopm_hash_table_t *h, ioopm_elem_t key);

///@brief remove a hash table from memory
///@param ht hash table to be destroyed
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht);

/// @brief returns the number of key => value entries in the hash table
/// @param h hash table operated upon
/// @return the number of key => value entries in the hash table
size_t ioopm_hash_table_size(ioopm_hash_table_t *h);

/// @brief checks if the hash table is empty
/// @param h hash table operated upon
/// @return true is size == 0, else false
bool ioopm_hash_table_is_empty(ioopm_hash_table_t *h);

/// @brief clear all the entries in a hash table
/// @param h hash table operated upon
void ioopm_hash_table_clear(ioopm_hash_table_t *h);

/// @brief return the keys for all entries in a hash map (in no particular order, but same as
////       ioopm_hash_table_values)
/// @param h hash table operated upon
/// @return a linked list of keys for hash table h
ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *h);

/// @brief return the values for all entries in a hash map
///        (in no particular order, but same as ioopm_hash_table_keys)
/// @param h hash table operated upon
/// @return an array of values for hash table h
ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *h);

/// @brief check if a hash table has an entry with a given key
/// @param h hash table operated upon
/// @param key the key sought
/// @return true if key exists, otherwise false
bool ioopm_hash_table_has_key(ioopm_hash_table_t *h, ioopm_elem_t key);

/// @brief check if a hash table has an entry with a given value
/// @param h hash table operated upon
/// @param value the value sought
/// @return true if value exists, otherwise false
bool ioopm_hash_table_has_value(ioopm_hash_table_t *h, ioopm_elem_t value);

/// @brief check if a predicate is satisfied by all entries in a hash table
/// @param h hash table operated upon
/// @param pred the predicate 
/// @param arg extra argument to pred
/// @return true if predicate is satisfied by all entries, otherwise false
bool ioopm_hash_table_all(ioopm_hash_table_t *h, ioopm_predicate pred, ioopm_elem_t arg);

/// @brief check if a predicate is satisfied by any entry in a hash table
/// @param h hash table operated upon
/// @param pred the predicate 
/// @param arg extra argument to pred
/// @return true if predicate is satisfied by all entries, otherwise false
bool ioopm_hash_table_any(ioopm_hash_table_t *h, ioopm_predicate pred, ioopm_elem_t arg);

/// @brief apply a function to all entries in a hash table
/// @param h hash table operated upon
/// @param apply_fun the function to be applied to all elements
/// @param arg extra argument to apply_fun
void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *h, ioopm_apply_function apply_fun, ioopm_elem_t arg);

