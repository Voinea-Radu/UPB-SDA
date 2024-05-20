#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "friends.h"
#include "utils/users.h"

#include "utils/Task1/graph.h"

void add_friend(char *name1, char *name2, graph_t *graph) {
	uint16_t id1 = get_user_id(name1);
	uint16_t id2 = get_user_id(name2);

	if ((id1 == MAX_UINT16) || (id2 == 65535)) {
		printf("User not found\n");
		return;
	}

	graph_add_edge(graph, id1, id2);
	graph_add_edge(graph, id2, id1);

	printf("Added connection %s - %s\n", name1, name2);
}

void remove_friend(char *name1, char *name2, graph_t *graph) {
	uint16_t id1 = get_user_id(name1);
	uint16_t id2 = get_user_id(name2);

	if ((id1 == MAX_UINT16) || (id2 == MAX_UINT16)) {
		printf("User not found\n");
		return;
	}

	graph_remove_edge(graph, id1, id2);
	graph_remove_edge(graph, id2, id1);

	printf("Removed connection %s - %s\n", name1, name2);
}

void get_distance(char *name1, char *name2, graph_t *graph) {
	uint16_t id1 = get_user_id(name1);
	uint16_t id2 = get_user_id(name2);

	if ((id1 == MAX_UINT16) || (id2 == MAX_UINT16)) {
		printf("User not found\n");
		return;
	}

	int16_t distance = graph_get_distance(graph, id1, id2);

	if (distance == -1)
		printf("There is no way to get from %s to %s\n", name1, name2);
	else
		printf("The distance between %s - %s is %d\n", name1, name2, distance);
}

int cmp_ids(void *a, void *b) {
	return *(uint16_t *)b - *(uint16_t *)a;
}

double_linked_list_t *__get_suggestions(graph_t *graph, uint16_t starting_id) {
	double_linked_list_t *suggestions = dll_list_init(sizeof(uint16_t), free);

	uint16_t *visited = calloc(graph->num_nodes, sizeof(uint16_t));
	queue_t *q = queue_create(sizeof(uint16_t), NULL);

	visited[starting_id] = 1;

	double_linked_list_t *from_list = graph->adjacency_list[starting_id];
	dll_node_t *curr = dll_get_head(from_list);

	while (curr != NULL) {
		uint16_t curr_data = *(uint16_t *)curr->data;

		if (!visited[curr_data]) {
			queue_enqueue(q, &curr_data);
			visited[curr_data] = 1;
		}

		curr = curr->next;
	}

	while (!queue_is_empty(q)) {
		uint16_t node = *(uint16_t *)queue_front(q);
		// TODO: free it
		dll_node_t *removed = queue_dequeue(q);

		double_linked_list_t *from_list = graph->adjacency_list[node];
		dll_node_t *curr = dll_get_head(from_list);

		while (curr != NULL) {
			uint16_t curr_data = *(uint16_t *)curr->data;

			if (!visited[curr_data]) {
				dll_list_add_sorted(suggestions, &curr_data, cmp_ids);
				visited[curr_data] = 1;
			}

			curr = curr->next;
		}
	}

	queue_free(q);
	free(visited);

	return suggestions;
}

void get_suggestions(char *name, graph_t *graph) {
	uint16_t id1 = get_user_id(name);

	if (id1 == MAX_UINT16) {
		printf("User not found\n");
		return;
	}

	double_linked_list_t *suggestions = __get_suggestions(graph, id1);

	if (dll_is_empty(suggestions))
		printf("There are no suggestions for %s\n", name);
	else {
		printf("Suggestions for %s:\n", name);

		dll_node_t *curr_node = dll_get_head(suggestions);

		for (size_t i = 0; i < suggestions->size; i++) {
			uint16_t *id = curr_node->data;
			char *user = get_user_name(*id);

			printf("%s\n", user);

			curr_node = curr_node->next;
		}
	}
}

uint16_t get_number_of_friends(uint16_t id, graph_t *graph) {
	if (id == MAX_UINT16) {
		printf("User not found\n");
		return 0;
	}

	return graph->adjacency_list[id]->size;
}

void handle_input_friends(char *input, graph_t *friends_graph) {
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");

	if (!cmd)
		return;

	if (!strcmp(cmd, "add")) {
		char *name1 = strtok(NULL, " ");
		char *name2 = strtok(NULL, "\n ");

		add_friend(name1, name2, friends_graph);
	} else if (!strcmp(cmd, "remove")) {
		char *name1 = strtok(NULL, " ");
		char *name2 = strtok(NULL, "\n ");

		remove_friend(name1, name2, friends_graph);
	} else if (!strcmp(cmd, "suggestions")) {
		char *name = strtok(NULL, "\n ");

		get_suggestions(name, friends_graph);
	} else if (!strcmp(cmd, "distance")) {
		char *name1 = strtok(NULL, " ");
		char *name2 = strtok(NULL, "\n ");

		get_distance(name1, name2, friends_graph);
	} else if (!strcmp(cmd, "friends")) {
		char *name = strtok(NULL, "\n ");
		uint16_t id = get_user_id(name);

		uint16_t number_of_friends = get_number_of_friends(id, friends_graph);
		printf("%s has %hu friends\n", name, number_of_friends);
	} else if (!strcmp(cmd, "popular")) {
		(void)cmd;
	} else if (!strcmp(cmd, "common")) {
		(void)cmd;
	}
}
