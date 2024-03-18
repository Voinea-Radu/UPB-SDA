/*
Nume: Voinea Radu-Mihai
Grupa: 315 CA
*/

#include "api/linked_list.h"
#include "api/utils.h"

linked_list_t *create_linked_list()
{
	linked_list_t* list = safe_malloc(sizeof(linked_list_t));
	list->size = 0;
	list->head = NULL;

	return list;
}

void add_node(linked_list_t *list, void *data)
{
	node_t *new_node = safe_malloc(sizeof(node_t));
	new_node->data = data;
	new_node->next = NULL;

	if (list->head == NULL) {
		list->head = new_node;
	} else {
		node_t *current = list->head;

		while (current->next != NULL) {
			current = current->next;
		}

		current->next = new_node;
	}

	list->size++;
}

void remove_node(linked_list_t *list, void *data)
{
	node_t *current = list->head;
	node_t *previous = NULL;

	while (current != NULL) {
		if (current->data == data) {
			if (previous == NULL) {
				list->head = current->next;
			} else {
				previous->next = current->next;
			}

			free(current);
			list->size--;
			return;
		}

		previous = current;
		current = current->next;
	}
}
