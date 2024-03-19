/*
Nume: Voinea Radu-Mihai
Grupa: 315 CA
*/

#include "api/linked_list.h"
#include "api/utils.h"

linked_list_t *create_linked_list(int8_t (*compare)(void *data1, void *data2))
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
	node_t *node = safe_malloc(sizeof(node_t));

	node->data = data;
	node->next = NULL;

	if (list->head == NULL) {
		list->head = node;
		list->tail = node;
	} else {
		list->tail->next = node;
		list->tail = node;
	}

	list->size++;
}

void add_node(linked_list_t *list, void *data)
{
	node_t *current_node = list->head;

	if (current_node == NULL) {
		add_node_at_tail(list, data);
		return;
	}

	if (list->compare(data, current_node->data) == -1) {
		node_t *new_node = safe_malloc(sizeof(node_t));

		new_node->data = data;
		new_node->next = current_node;
		new_node->prev = NULL;

		current_node->prev = new_node;

		list->head = new_node;
		list->size++;

		return;
	}

	while (current_node != NULL) {
		void *current_data = current_node->data;
		void *next_data = current_node->next == NULL ? NULL : current_node->next->data;

		bool result1 = list->compare(current_data, data) == -1;
		bool result2 = 0;
		if(next_data== NULL) {
			result2 = 1;
		}else{
			result2 = list->compare(data, next_data) == -1;
		}


		if (result1 && ((next_data == NULL) || result2)) {
			node_t *next_block_node = current_node->next;

			node_t *block_node = safe_malloc(sizeof(node_t));

			block_node->data = data;
			block_node->next = current_node->next;
			block_node->prev = current_node;

			current_node->next = block_node;

			if (next_block_node != NULL) {
				next_block_node->prev = block_node;
			}

			list->size++;

			return;
		}

		current_node = current_node->next;
	}

	add_node_at_tail(list, data);
}

void remove_node(linked_list_t *list, void *data)
{
	node_t *current = list->head;
	node_t *prev = NULL;

	while (current != NULL) {
		if (current->data == data) {
			if (prev == NULL) {
				list->head = current->next;
			} else {
				prev->next = current->next;
			}

			if (current->next == NULL) {
				list->tail = prev;
			}

			free(current);
			list->size--;

			return;
		}

		prev = current;
		current = current->next;
	}


}