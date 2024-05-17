/*
 * Copyright (c) 2024 Coman Andrei-Madalin <andrei.coman1301@stud.acs.upb.ro>
 */

#include "graph.h"

graph_t *graph_create(size_t num_nodes, size_t data_size,
					  void (*free_data_function)(void *)) {
	graph_t *graph = malloc(sizeof(graph_t));

	graph->num_nodes = num_nodes;
	graph->adjacency_list = malloc(num_nodes * sizeof(double_linked_list_t *));

	for (size_t i = 0; i < num_nodes; i++)
		graph->adjacency_list[i] = dll_list_init(data_size, free_data_function);

	return graph;
}

void graph_add_edge(graph_t *graph, size_t from, size_t to) {
	double_linked_list_t *from_list = graph->adjacency_list[from];
	dll_add_nth_node(from_list, dll_get_size(from_list), &to);
}

bool graph_has_edge(graph_t *graph, size_t from, size_t to) {
	double_linked_list_t *from_list = graph->adjacency_list[from];
	dll_node_t *node = dll_get_head(from_list);

	while (node != NULL) {
		uint16_t node_data = *(uint16_t *)node->data;

		if (node_data == to)
			return true;

		node = node->next;
	}

	return false;
}

void graph_remove_edge(graph_t *graph, size_t from, size_t to) {
	double_linked_list_t *from_list = graph->adjacency_list[from];
	dll_node_t *node = dll_get_head(from_list);
	size_t index = 0;

	while (node != NULL) {
		uint16_t node_data = *(uint16_t *)node->data;

		if (node_data == to) {
			dll_remove_nth_node(from_list, index);
			break;
		}

		node = node->next;
		index++;
	}
}

void graph_free(graph_t *graph) {
	for (size_t i = 0; i < graph->num_nodes; i++)
		dll_list_free(graph->adjacency_list[i]);

	free(graph->adjacency_list);
	free(graph);
}