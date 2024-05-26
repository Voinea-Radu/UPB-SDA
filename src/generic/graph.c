/*
 * Copyright (c) 2024 Coman Andrei-Madalin <andrei.coman1301@stud.acs.upb.ro>
 */

#include "graph.h"

graph_t *graph_create(size_t num_nodes, size_t data_size,
					  void (*free_data_function)(void *))
{
	graph_t *graph = malloc(sizeof(graph_t));

	graph->num_nodes = num_nodes;
	graph->adjacency_list = malloc(num_nodes * sizeof(double_linked_list_t *));

	for (size_t i = 0; i < num_nodes; i++)
		graph->adjacency_list[i] = dll_list_init(data_size, free_data_function);

	return graph;
}

void graph_add_edge_sorted(graph_t *graph, size_t from, size_t to,
						   int (*cmp)(void *, void *))
{
	double_linked_list_t *from_list = graph->adjacency_list[from];
	dll_list_add_sorted(from_list, &to, cmp);
}

void graph_remove_edge(graph_t *graph, size_t from, size_t to)
{
	double_linked_list_t *from_list = graph->adjacency_list[from];
	dll_node_t *node = dll_get_head(from_list);
	size_t index = 0;

	while (node) {
		uint16_t node_data = *(uint16_t *)node->data;

		if (node_data == to) {
			dll_node_t *to_free = dll_remove_nth_node(from_list, index);
			free(to_free->data);
			free(to_free);
			break;
		}

		node = node->next;
		index++;
	}
}

void graph_free(graph_t *graph)
{
	for (size_t i = 0; i < graph->num_nodes; i++)
		dll_list_free(graph->adjacency_list[i]);

	free(graph->adjacency_list);
	free(graph);
}
