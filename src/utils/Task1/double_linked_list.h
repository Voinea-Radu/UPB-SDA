/*
 * Copyright (c) 2024 Coman Andrei-Madalin <andrei.coman1301@stud.acs.upb.ro>
 */

#ifndef DOUBLE_LINKED_LIST_H
#define DOUBLE_LINKED_LIST_H

#include "mem_alloc_check.h"	// for DIE
#include <stdlib.h>		// for malloc, calloc
#include <stdbool.h>    // for bool
#include <stdio.h> 		// for printf
#include <string.h>		// for memcpy

typedef struct dll_node_t {
	void *data;
	struct dll_node_t *prev, *next;
} dll_node_t;

typedef struct {
	dll_node_t *head;
	dll_node_t *tail;
	size_t size;
	size_t data_size;

	void (*free_data_function)(void *);
} double_linked_list_t;

dll_node_t *dll_node_alloc(void);

double_linked_list_t *dll_list_init(size_t data_size, void *free_data_function);

size_t dll_get_size(double_linked_list_t *list);

bool dll_is_empty(double_linked_list_t *list);

void dll_add_nth_node(double_linked_list_t *list, size_t n, const void *data);

dll_node_t *dll_remove_nth_node(double_linked_list_t *list, size_t n);

dll_node_t *dll_get_head(double_linked_list_t *list);

dll_node_t *dll_get_tail(double_linked_list_t *list);

dll_node_t *dll_remove_head(double_linked_list_t *list);

void dll_add_tail(double_linked_list_t *list, dll_node_t *node);

void dll_refresh_node(double_linked_list_t *list, dll_node_t *node);

void dll_list_free(double_linked_list_t *list);

dll_node_t *dll_list_add_sorted(double_linked_list_t *list, void *value,
								int (*cmp)(void *, void *));

dll_node_t *dll_list_remove_custom(double_linked_list_t *list, void *value,
								   int (*cmp)(void *, void *));

#endif /* DOUBLE_LINKED_LIST_H */
