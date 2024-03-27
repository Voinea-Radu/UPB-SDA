/*
Nume: Voinea Radu-Mihai
Grupa: 315 CA
*/

#ifndef TEMA1_LINKED_LIST_H
#define TEMA1_LINKED_LIST_H

#include "utils.h"

typedef struct node {
	void *data;

	struct node *prev;
	struct node *next;
} node_t;

typedef struct {
	s64 size;

	node_t *head;
	node_t *tail;

	s8 (*compare)(void *data1, void *data2);
} linked_list_t;

linked_list_t *new_linked_list(s8 (*compare)(void *data1, void *data2));

void add_node_at_tail(linked_list_t *list, void *data);

void add_node_sorted(linked_list_t *list, void *data);

void add_node_at(linked_list_t *list, void *data, s64 index);

s64 get_node_index(linked_list_t *list, void *data);

node_t *get_node_at(linked_list_t *list, s64 index);

node_t *remove_node(linked_list_t *list, void *data);

node_t *remove_node_at(linked_list_t *list, s64 index);

#endif //TEMA1_LINKED_LIST_H
