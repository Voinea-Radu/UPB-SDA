//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>
#include "../utils/constants.h"

typedef struct node {
	void *data;
	struct node *next;
} node_t;

typedef struct {
	node_t *head;
	node_t *tail;

	uint size;

	uint (*data_get_size)(void *value);
	bool (*data_compare)(void *data1, void *data2);
	void (*data_free)(void **data);
} linked_list_t;

// ==================== Constructor(s) ====================

linked_list_t *linked_list_init(uint (*data_get_size)(void *value), bool (*data_compare)(void *data1, void *data2),
								void (*data_free)(void **data));

// ==================== Memory ====================

void linked_list_free(linked_list_t **list);

// ==================== Functional ====================

void linked_list_add(linked_list_t *list, void *data);

void linked_list_add_at_index(linked_list_t *list, void *data, int index);

void* linked_list_get_at_index(linked_list_t *list, int index);

void* linked_list_remove(linked_list_t *list, void *data);

void linked_list_print(linked_list_t *list, void (*print_data)(void *data));

#endif //LINKED_LIST_H
