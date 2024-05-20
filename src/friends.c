#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "friends.h"
#include "utils/users.h"

#include "utils/Task1/graph.h"

void add_friend(char *name1, char *name2, graph_t *graph) {
	uint16_t id1 = get_user_id(name1);
	uint16_t id2 = get_user_id(name2);

	if ((id1 == 65535) || (id2 == 65535)) {
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

	if (!id1 || !id2) {
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

	if (!id1 || !id2) {
		printf("User not found\n");
		return;
	}

	int16_t distance = graph_get_distance(graph, id1, id2);

	if (distance == -1)
		printf("There is no way to get from %s to %s\n", name1, name2);
	else
		printf("The distance between %s - %s is %d\n", name1, name2, distance);
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
		(void)cmd;
		// TODO: Add function
	} else if (!strcmp(cmd, "distance")) {
		char *name1 = strtok(NULL, " ");
		char *name2 = strtok(NULL, "\n ");

		get_distance(name1, name2, friends_graph);
	} else if (!strcmp(cmd, "friends")) {
		(void)cmd;
	} else if (!strcmp(cmd, "popular")) {
		(void)cmd;
	} else if (!strcmp(cmd, "common")) {
		(void)cmd;
	} else if (!strcmp(cmd, "friends")) {
		(void)cmd;
	} else if (!strcmp(cmd, "popular")) {
		(void)cmd;
		// TODO: Add function
	}
}
