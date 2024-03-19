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
	list->tail = NULL;

	return list;
}

void add_node(linked_list_t *list, void *data)
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