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

void graph_add_edge_sorted(graph_t *graph, size_t from, size_t to,
						   int (*cmp)(void *, void *)) {
	double_linked_list_t *from_list = graph->adjacency_list[from];
	dll_list_add_sorted(from_list, &to, cmp);
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

int16_t
graph_get_distance(graph_t *graph, uint16_t curr_node, uint16_t find_node) {
	int *dist = calloc(graph->num_nodes, sizeof(int));
	int *visited = calloc(graph->num_nodes, sizeof(int));
	queue_t *q = queue_create(sizeof(uint16_t), NULL);

	queue_enqueue(q, &curr_node);
	visited[curr_node] = 1;
	dist[curr_node] = 0;

	while (!queue_is_empty(q)) {
		size_t node = *(size_t *)queue_front(q);
		dll_node_t *removed = queue_dequeue(q);
//		char *user = get_user_name(node);
//		printf("user: %s\n", user);

		double_linked_list_t *neighbours = graph->adjacency_list[node];
		dll_node_t *curr = dll_get_head(neighbours);

		while (curr) {
			size_t neighbour = *(size_t *)curr->data;

			char *neighbour_user = get_user_name(neighbour);

			if (!visited[neighbour]) {
				visited[neighbour] = 1;
				dist[neighbour] = dist[node] + 1;
				queue_enqueue(q, &neighbour);
			}

			if (neighbour == find_node) {
				int ret = dist[neighbour];

//				printf("user: %s\n", get_user_name(neighbour));

//				free(dist);
//				free(visited);
//				queue_free(q);

				return ret;
			}

			curr = curr->next;
		}
	}

	free(dist);
	free(visited);
	queue_free(q);

	return -1;
}