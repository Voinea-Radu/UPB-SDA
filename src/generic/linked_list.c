//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include "linked_list.h"
#include "../utils/utils.h"

linked_list_t *linked_list_init(void (*free)(void *), bool (*compare)(void*, void*))
{
	linked_list_t *list = safe_malloc(sizeof(linked_list_t));

	list->head = NULL;
	list->tail = NULL;
	list->size = 0;

	list->free = free;
	list->compare = compare;

	return list;
}

void linked_list_add(linked_list_t *list, void *data)
{
	node_t *node = safe_malloc(sizeof(node_t));
	node->data = data;
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

void linked_list_remove(linked_list_t *list, void *data)
{
	node_t *current = list->head;
	node_t *previous = NULL;

	while (current != NULL) {
		if (list->compare(current->data, data)) {
			if (previous == NULL) {
				list->head = current->next;
			} else {
				previous->next = current->next;
			}

			if (current == list->tail) {
				list->tail = previous;
			}

			free(current);
			list->size--;
			return;
		}

		previous = current;
		current = current->next;
	}
}

void linked_list_free(linked_list_t *list)
{
	node_t *current = list->head;
	node_t *next = NULL;

	while (current != NULL) {
		next = current->next;
		free(current);
		current = next;
	}

	free(list);
}

void linked_list_print_prefixed(linked_list_t *list, string_t prefix, void (*print)(string_t, void*)){
	node_t *current = list->head;

	while (current != NULL) {
		void* data = current->data;
		print(prefix, data);
		current = current->next;
	}
}


void linked_list_print(linked_list_t *list, void (*print)(void *data))
{
	node_t *current = list->head;

	while (current != NULL) {
		print(current->data);
		current = current->next;
	}
}

void for_each(linked_list_t *list, void (*callback)(void *data))
{
	node_t *current = list->head;

	while (current != NULL) {
		callback(current->data);
		current = current->next;
	}
}

bool linked_list_contains(linked_list_t *list, void *data)
{
	node_t *current = list->head;

	while (current != NULL) {
		if (list->compare(current->data, data)) {
			return true;
		}

		current = current->next;
	}

	return false;
}
