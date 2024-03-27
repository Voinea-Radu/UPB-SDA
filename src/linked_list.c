/*
Nume: Voinea Radu-Mihai
Grupa: 315 CA
*/

#include "api/linked_list.h"
#include "api/utils.h"

linked_list_t *new_linked_list(s8 (*compare)(void *data1, void *data2))
{
	linked_list_t *list = safe_malloc(sizeof(linked_list_t));

	list->size = 0;

	list->head = NULL;
	list->tail = NULL;

	list->compare = compare;

	return list;

}

void add_node_at_tail(linked_list_t *list, void *data)
{
	add_node_at(list, data, list->size);
}

void add_node_sorted(linked_list_t *list, void *data)
{
	node_t *node = safe_malloc(sizeof(node_t));
	node->data = data;

	if (list->size == 0) {
		list->head = node;
		list->tail = node;
		node->prev = NULL;
		node->next = NULL;
		list->size++;
		return;
	}

	node_t *current_node = list->head;

	while (current_node != NULL) {
		if (list->compare(current_node->data, data) > 0) {
			if (current_node->prev == NULL) {
				node->next = current_node;
				node->prev = NULL;
				current_node->prev = node;
				list->head = node;
				list->size++;
				return;
			}

			node->next = current_node;
			node->prev = current_node->prev;
			current_node->prev->next = node;
			current_node->prev = node;
			list->size++;
			return;
		}

		if (current_node->next == NULL) {
			node->next = NULL;
			node->prev = current_node;
			current_node->next = node;
			list->tail = node;
			list->size++;
			return;
		}

		current_node = current_node->next;
	}
}

void add_node_at(linked_list_t *list, void *data, s64 index)
{
	index = clamp(index, 0, list->size);
	node_t *node = safe_malloc(sizeof(node_t));

	node->data = data;

	if (list->size == 0) {
		list->head = node;
		list->tail = node;
		node->prev = NULL;
		node->next = NULL;
		list->size++;
		return;
	}

	if (index == 0) {
		node->next = list->head;
		node->prev = NULL;
		list->head->prev = node;
		list->head = node;
		list->size++;
		return;
	}

	if (index == list->size) {
		node->next = NULL;
		node->prev = list->tail;
		list->tail->next = node;
		list->tail = node;
		list->size++;
		return;
	}

	node_t *current_node = list->head;
	s64 i = 0;

	while (i < index) {
		current_node = current_node->next;
		i++;
	}

	node->next = current_node;
	node->prev = current_node->prev;
	current_node->prev->next = node;
	current_node->prev = node;
	list->size++;

}

s64 get_node_index(linked_list_t *list, void *data)
{
	node_t *current_node = list->head;
	s64 i = 0;

	while (current_node != NULL) {
		if (list->compare(current_node->data, data) == 0) {
			return i;
		}
		current_node = current_node->next;
		i++;
	}

	return -1;

}

node_t *get_node_at(linked_list_t *list, s64 index)
{
	index = clamp(index, 0, list->size);

	node_t *current_node = list->head;
	s64 i = 0;

	while (i < index) {
		current_node = current_node->next;
		i++;
	}

	return current_node;

}

node_t *remove_node(linked_list_t *list, void *data)
{
	s64 index = get_node_index(list, data);
	return remove_node_at(list, index);
}

node_t *remove_node_at(linked_list_t *list, s64 index)
{
	index = clamp(index, 0, list->size);

	node_t *current_node = list->head;
	s64 i = 0;

	while (i < index) {
		current_node = current_node->next;
		i++;
	}

	if (current_node->prev == NULL) {
		list->head = current_node->next;
	} else {
		current_node->prev->next = current_node->next;
	}

	if (current_node->next == NULL) {
		list->tail = current_node->prev;
	} else {
		current_node->next->prev = current_node->prev;
	}

	list->size--;

	return current_node;
}