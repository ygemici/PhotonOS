#ifndef _list_h
#define _list_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <phapi.h>
#include <sys/cdefs.h>

typedef int8_t (*lessthan_pred_t)(type_t, type_t);

typedef struct {
	type_t *array;
	size_t size;
	size_t max_size;
	lessthan_pred_t lessthan;
} list_t;


/**
  * Standard 'less than' checker.
  */
int8_t std_lessthan_pred(type_t a, type_t b);

/**
  * Create an ordered array.
  */
list_t create_list(size_t max_size, lessthan_pred_t lessthan);

/**
  * Place an ordered array at specified location.
  */
list_t place_list(void *addr, size_t max_size, lessthan_pred_t lessthan);

/**
  * Destroy the ordered array.
  */
void destroy_list(list_t *array);

/**
  * Insert an item in the array.
  */
void insert_list(type_t item, list_t *array);

/**
  * Look up at index 'i' for item.
  */
type_t lookup_list(size_t i, list_t *array);

/**
  * Remove item from array.
  */
void remove_list(size_t i, list_t *array);

#endif
