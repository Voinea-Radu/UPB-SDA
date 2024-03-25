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

linked_list_t *new_linked_list(int8_t (*compare)(void *data1, void *data2));

void add_node_at_tail(linked_list_t *list, void *data);

void add_node_sorted(linked_list_t *list, void *data);

void add_node_at(linked_list_t *list, void *data, int64_t index);

int64_t get_node_index(linked_list_t *list, void *data);

node_t *get_node_at(linked_list_t *list, int64_t index);

node_t* remove_node(linked_list_t *list, void *data);

node_t* remove_node_at(linked_list_t *list, int64_t index);

#endif //TEMA1_LINKED_LIST_H
