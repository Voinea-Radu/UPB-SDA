/*
Nume: Voinea Radu-Mihai
Grupa: 315 CA
*/

#ifndef TEMA1_LINKED_LIST_H
#define TEMA1_LINKED_LIST_H

#include <stdint.h>

typedef struct node {
	void *data;

	struct node *prev;
	struct node *next;
} node_t;

typedef struct {
	int64_t size;

	node_t *head;
	node_t *tail;

	int8_t (*compare)(void *data1, void *data2);
} linked_list_t;

linked_list_t *create_linked_list(int8_t (*compare)(void *data1, void *data2));

void add_node_at_tail(linked_list_t *list, void *data);

void add_node(linked_list_t *list, void *data);

node_t* remove_node(linked_list_t *list, void *data);

#endif //TEMA1_LINKED_LIST_H
