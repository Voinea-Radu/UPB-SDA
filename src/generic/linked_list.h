//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "../utils/constants.h"

typedef struct node_t_prototype {
	void *data;
	struct node_t_prototype *next;
} node_t;

typedef struct {
	node_t *head;
	node_t *tail;

	int size;
} linked_list_t;

linked_list_t *linked_list_init();

void linked_list_add(linked_list_t *list, void *data);

void linked_list_remove(linked_list_t *list, void *data);

void linked_list_free(linked_list_t *list);

void linked_list_print_prefixed(linked_list_t *list, string_t prefix, void (*print)(void *data, string_t));

void linked_list_print(linked_list_t *list, void (*print)(void *data));

#endif // LINKED_LIST_H
