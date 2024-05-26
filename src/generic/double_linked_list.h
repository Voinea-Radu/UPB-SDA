/*
 * Copyright (c) 2024 Coman Andrei-Madalin <andrei.coman1301@stud.acs.upb.ro>
 */

#ifndef DOUBLE_LINKED_LIST_H
#define DOUBLE_LINKED_LIST_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct dll_node_t {
	void *data;
	struct dll_node_t *prev, *next;
} dll_node_t;

typedef struct {
	dll_node_t *head;
	dll_node_t *tail;
	size_t size;
	size_t data_size;

	void (*free_data_function)(void *data);
} double_linked_list_t;

dll_node_t *dll_node_alloc(void);

double_linked_list_t *dll_list_init(size_t data_size, void *free_data_function);

size_t dll_get_size(double_linked_list_t *list);

bool dll_is_empty(double_linked_list_t *list);

dll_node_t *dll_remove_nth_node(double_linked_list_t *list, size_t n);

dll_node_t *dll_get_head(double_linked_list_t *list);

dll_node_t *dll_remove_head(double_linked_list_t *list);

dll_node_t *dll_remove_tail(double_linked_list_t *list);

void dll_add_head(double_linked_list_t *list, void *value);

void dll_add_tail(double_linked_list_t *list, dll_node_t *node);

void dll_add_tail_value(double_linked_list_t *list, void *value);

void dll_list_free(double_linked_list_t *list);

dll_node_t *dll_list_add_sorted(double_linked_list_t *list, void *value,
								int (*cmp)(void *data1, void *data2));

dll_node_t *
dll_list_add_sorted_with_limit(double_linked_list_t *list, void *value,
							   int (*cmp)(void *data1, void *data2),
							   size_t limit);

#endif /* DOUBLE_LINKED_LIST_H */
