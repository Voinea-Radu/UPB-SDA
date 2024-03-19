/*
Nume: Voinea Radu-Mihai
Grupa: 315 CA
*/

#ifndef TEMA1_LINKED_LIST_H
#define TEMA1_LINKED_LIST_H

#include <stdint.h>

typedef struct node {
	void *data;
	struct node *next;
} node_t;

typedef struct {
	uint64_t size;
	node_t *head;
} linked_list_t;

linked_list_t *create_linked_list();

void add_node(linked_list_t *list, void *data);

void remove_node(linked_list_t *list, void *data);

void remove_node_at(linked_list_t *list, uint64_t index);

#endif //TEMA1_LINKED_LIST_H
