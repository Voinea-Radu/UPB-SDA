/*
 * Copyright (c) 2024 Coman Andrei-Madalin <andrei.coman1301@stud.acs.upb.ro>
 */

#ifndef GRAPH_H
#define GRAPH_H

#include "double_linked_list.h"
#include <stdbool.h>
#include <stdint.h>
#include "queue.h"
#include "../users.h"

typedef struct Graph {
	size_t num_nodes;
	double_linked_list_t **adjacency_list;
} graph_t;

graph_t *graph_create(size_t num_nodes, size_t data_size,
					  void (*free_data_function)(void *));

void graph_add_edge(graph_t *graph, size_t from, size_t to);

void graph_add_edge_sorted(graph_t *graph, size_t from, size_t to,
						   int (*cmp)(void *, void *));

bool graph_has_edge(graph_t *graph, size_t from, size_t to);

void graph_remove_edge(graph_t *graph, size_t from, size_t to);

void graph_free(graph_t *graph);

int16_t graph_get_distance(graph_t *graph, uint16_t curr_node, uint16_t find_node);

#endif /* GRAPH_H */