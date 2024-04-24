//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include <stdlib.h>
#include "linked_list.h"
#include "../utils/utils.h"

linked_list_t *linked_list_init(uint (*data_get_size)(void *value), bool (*data_compare)(void *data1, void *data2),
								void (*data_free)(void **data))
{
	linked_list_t *list = malloc(sizeof(linked_list_t));
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;

	list->data_get_size = data_get_size;
	list->data_compare = data_compare;
	list->data_free = data_free;

	return list;
}

void linked_list_free(linked_list_t **list)
{
	node_t *node = (*list)->head;
	node_t *next;

	while (node) {
		next = node->next;
		(*list)->data_free(&node->data);
		free(node);
		node = next;
	}

	free(*list);
	*list = NULL;
}

void linked_list_add_at_index(linked_list_t *list, void *data, int index)
{
	node_t *node = malloc(sizeof(node_t));
	node->data = create_and_copy(data, list->data_get_size);
	node->next = NULL;

	if (list->size == 0) {
		list->head = node;
		list->tail = node;
	} else {
		node_t *current = list->head;
		node_t *prev = NULL;
		int i = 0;

		while (current && i < index) {
			prev = current;
			current = current->next;
			i++;
		}

		if (prev) {
			prev->next = node;
		} else {
			list->head = node;
		}

		node->next = current;

		if (current == NULL) {
			list->tail = node;
		}
	}

	list->size++;

}

void linked_list_add(linked_list_t *list, void *data)
{
	node_t *node = malloc(sizeof(node_t));
	node->data = create_and_copy(data, list->data_get_size);
	node->next = NULL;

	if (list->size == 0) {
		list->head = node;
		list->tail = node;
	} else {
		list->tail->next = node;
		list->tail = node;
	}

	list->size++;
}

void *linked_list_remove(linked_list_t *list, void *data)
{
	node_t *node = list->head;
	node_t *prev = NULL;

	while (node) {
		if (list->data_compare(node->data, data)) {
			if (prev) {
				prev->next = node->next;
			} else {
				list->head = node->next;
			}

			if (node == list->tail) {
				list->tail = prev;
			}

			void* output = node->data;

			free(node);
			list->size--;

			return output;
		}

		prev = node;
		node = node->next;
	}

	return NULL;
}

void linked_list_print(linked_list_t *list, void (*print_data)(void *data))
{
	node_t *node = list->head;

	while (node) {
		print_data(node->data);
		node = node->next;
	}
}
