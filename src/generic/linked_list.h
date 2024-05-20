//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>

#include "../utils/constants.h"

typedef struct node_t_prototype {
	void *data;
	struct node_t_prototype *next;
} node_t;

typedef struct {
	node_t *head;
	node_t *tail;

	int size;

	void (*free)(void *);

	bool (*compare)(void *, void *);

} linked_list_t;

linked_list_t *linked_list_init(void (*free)(void *), bool (*compare)(void *, void *));

void linked_list_add(linked_list_t *list, void *data);

void linked_list_remove(linked_list_t *list, void *data);

void linked_list_free(linked_list_t *list);

// TODO move to debug class
void linked_list_print_prefixed(linked_list_t *list, string_t prefix, void (*print)(string_t, void *));

// TODO move to debug class
void linked_list_print(linked_list_t *list, void (*print)(void *));

void for_each(linked_list_t *list, void (*callback)(void *));

bool linked_list_contains(linked_list_t *list, void *data);

#endif // LINKED_LIST_H
