#include <string.h>

#include "feed.h"
#include "../utils/users.h"
#include "../utils/constants.h"
#include "../friends.h"
#include "../task2/task2.h"
#include "../task2/post.h"
#include "../task1/stack.h"

void print_profile(string_t username);
void __print_reposting_friends(linked_list_t *posts, linked_list_t *friends);
void friends_repost(string_t username, uint32_t post_id);
void __get_all_posts_for_user(linked_list_t *posts, uint16_t user_id, uint32_t *output, uint32_t *size);

void show_feed(graph_t *friends_graph, char *name, int size);
void find_clique(graph_t *friends_graph, char *name);

void handle_input_feed(char *input) {
	char *command = strtok(input, "\n ");

	if (!command) {
		return;
	}

	if (!strcmp(command, "feed")) {
		char *name = strtok(NULL, "\n ");

		char *size_str = strtok(NULL, "\n ");
		int size = atoi(size_str);
		if (size < 0)
			return;

		graph_t *friends = get_all_friends();
		show_feed(friends, name, size);
	} else if (!strcmp(command, "view-profile")) {
		string_t username = strtok(NULL, "\n ");

		print_profile(username);
	} else if (!strcmp(command, "friends-repost")) {
		string_t username = strtok(NULL, " ");
		uint32_t post_id = atoi(strtok(NULL, "\n "));

		friends_repost(username, post_id);
	} else if (!strcmp(command, "common-group")) {
		char *name = strtok(NULL, "\n ");

		graph_t *friends = get_all_friends();
		find_clique(friends, name);
	}
}

void show_feed(graph_t *friends_graph, char *name, int size) {
	uint16_t id = get_user_id(name);
	if (id == MAX_UINT16) {
		printf("User not found\n");
		return;
	}

	linked_list_t *posts = get_all_posts();

	double_linked_list_t *friends = friends_graph->adjacency_list[id];

	double_linked_list_t *feed = posts_get_by_user_id(posts, friends, id, size);

	dll_node_t *curr_node = dll_get_head(feed);
	for (size_t i = 0; i < feed->size; i++) {
		post_t *post = curr_node->data;

		char *username = get_username(post->user_id);
		printf("%s: %s\n", username, post->title);
		dll_node_t *to_free = curr_node;
		curr_node = curr_node->next;

		free(to_free);
	}

	free(feed);
}

void __get_all_posts_for_user(linked_list_t *posts, uint16_t user_id, uint32_t *output, uint32_t *size) {
	node_t *current = posts->head;

	while (current != NULL) {
		post_t *post = (post_t *)current->data;

		if (post->user_id == user_id) {
			output[(*size)++] = post->id;
		}

		__get_all_posts_for_user(post->reposts, user_id, output, size);
		current = current->next;
	}
}

void print_profile(string_t username) {
	uint16_t id = get_user_id(username);
	uint32_t *output = safe_malloc(get_all_posts()->size);
	uint32_t output_size = 0;

	__get_all_posts_for_user(get_all_posts(), id,output, &output_size);

	// sort output
	for (uint32_t i = 0; i < output_size; i++) {
		for (uint32_t j = i + 1; j < output_size; j++) {
			if (output[i] > output[j]) {
				uint32_t temp = output[i];
				output[i] = output[j];
				output[j] = temp;
			}
		}
	}

	for (uint32_t i = 0; i < output_size; i++) {
		post_t *post = post_get(get_all_posts(), output[i], 0);
		if (post->is_repost) {
			printf("Reposted: %s\n", post->title);
		} else {
			printf("Posted: %s\n", post->title);
		}
	}

	free(output);
}

void __print_reposting_friends(linked_list_t *posts, linked_list_t *friends) {
	node_t *current = posts->head;
	while (current != NULL) {
		post_t *post = (post_t *)current->data;
		uint16_t user_id = post->user_id;

		if (linked_list_contains(friends, &user_id)) {
			printf("%s\n", get_username(user_id));
		}

		__print_reposting_friends(post->reposts, friends);
		current = current->next;
	}
}

void friends_repost(string_t username, uint32_t post_id) {
	uint16_t user_id = get_user_id(username);
	post_t *post = post_get(get_all_posts(), post_id, 0);

	double_linked_list_t *__friends = get_friends(get_all_friends(), user_id);

	linked_list_t *friends = linked_list_init((void (*)(void *))free_na,
											  compare_int);

	dll_node_t *current = __friends->head;

	while (current != NULL) {
		linked_list_add(friends, current->data);
		current = current->next;
	}

	__print_reposting_friends(post->reposts, friends);

	linked_list_free(friends);
}

int compare_uint16(void *a, void *b) {
	return *(uint16_t *)b - *(uint16_t *)a;
}

void check_if_bigger(double_linked_list_t **clique,
					 double_linked_list_t *curr_clique) {
	if (curr_clique->size > (*clique)->size) {
		dll_list_free(*clique);
		*clique = dll_list_init(sizeof(uint16_t), free);

		dll_node_t *curr_node = dll_get_head(curr_clique);
		for (size_t i = 0; i < curr_clique->size; i++) {
			uint16_t friend = *(uint16_t *)curr_node->data;
			dll_list_add_sorted((*clique), &friend, compare_uint16);
			curr_node = curr_node->next;
		}
	}
}

void dfs_recursive(graph_t *friends_graph, bool *visited,
				   double_linked_list_t **clique, double_linked_list_t
				   *possible_clique, double_linked_list_t *checked_users,
				   uint16_t current, bool *done) {
	visited[current] = true;
	stack_t *stack = stack_init(sizeof(uint16_t), free);

	double_linked_list_t *friends = friends_graph->adjacency_list[current];

	dll_node_t *curr_clique_node = dll_get_head(possible_clique);
	dll_node_t *curr_friend_node = dll_get_head(friends);

	for (size_t i = 0; i < possible_clique->size; i++) {
		uint16_t in_clique = *(uint16_t *)curr_clique_node->data;

		if (in_clique == current) {
			curr_clique_node = curr_clique_node->next;
			continue;
		}

		bool found = false;

		while (1) {
			if(curr_friend_node == NULL)
				break;

			uint16_t friend = *(uint16_t *)curr_friend_node->data;

			if (in_clique == friend) {
				found = true;
				if (!visited[friend])
					stack_push(stack, &friend);
				break;
			}

			if (in_clique > friend) {
				curr_friend_node = curr_friend_node->next;
				continue;
			}

			if (in_clique < friend) {
				break;
			}
		}

		if (!found) {
			curr_clique_node = curr_clique_node->next;
			dll_node_t *to_free = dll_remove_nth_node(possible_clique, i);
			possible_clique->free_data_function(to_free->data);
			free(to_free);
			i--;
			continue;
		}

		curr_clique_node = curr_clique_node->next;
	}

//	TODO: free everything
	if(possible_clique->size == 0) {
		return;
	}

	dll_list_add_sorted(checked_users, &current, compare_uint16);
	check_if_bigger(clique, checked_users);

	if(possible_clique->size == checked_users->size) {
		bool is_same = true;

		dll_node_t *curr_poss_node = dll_get_head(possible_clique);
		dll_node_t *curr_checked_node = dll_get_head(checked_users);
		for (size_t i = 0; i < possible_clique->size; i++) {
			uint16_t poss = *(uint16_t *)curr_poss_node->data;
			uint16_t checked = *(uint16_t *)curr_checked_node->data;

			if(poss != checked) {
				is_same = false;
				break;
			}

			curr_poss_node = curr_poss_node->next;
			curr_checked_node = curr_checked_node->next;
		}

		if(is_same)
		{
			*done = true;
			return;
		}
	}

	while (!stack_is_empty(stack)) {
		uint16_t *friend = stack_pop(stack);
		if (!visited[*friend]) {
			double_linked_list_t *new_clique = dll_list_init(sizeof(uint16_t),
															  free);
			dll_node_t *curr_node = dll_get_head(possible_clique);
			for (size_t i = 0; i < possible_clique->size; i++) {
				uint16_t poss_mem = *(uint16_t *)curr_node->data;
				dll_list_add_sorted(new_clique, &poss_mem, compare_uint16);
				curr_node = curr_node->next;
			}
			//dll_list_add_sorted(new_clique, friend, compare_uint16);

			dfs_recursive(friends_graph, visited, clique, new_clique,
						  checked_users, *friend, done);

			if(*done)
				return;

			dll_list_free(new_clique);
		}

		free(friend);
	}

}

void find_clique(graph_t *friends_graph, char *name) {
	uint16_t id = get_user_id(name);
	if (id == MAX_UINT16) {
		printf("User not found\n");
		return;
	}

	double_linked_list_t *clique = dll_list_init(sizeof(uint16_t), free);

	bool *visited = safe_calloc(friends_graph->num_nodes * sizeof(bool));
	visited[id] = true;

	double_linked_list_t *friends = friends_graph->adjacency_list[id];
	dll_node_t *friends_node = friends->head;
	for (size_t i = 0; i < friends->size; i++) {
		uint16_t friend = *(uint16_t *)friends_node->data;

		if(visited[friend]) {
			friends_node = friends_node->next;
			continue;
		}

//		Init everything
		double_linked_list_t *curr_clique = dll_list_init(sizeof(uint16_t),
														  free);
		double_linked_list_t *checked_users = dll_list_init(sizeof(uint16_t),
															free);
		dll_add_tail_value(checked_users, &id);

		dll_node_t *curr_node = dll_get_head(friends);
//	Initially, the clique is formed by all the friends of the user + the user
		for (size_t j = 0; j < friends->size; j++) {
			uint16_t poss_mem = *(uint16_t *)curr_node->data;
			dll_list_add_sorted(curr_clique, &poss_mem, compare_uint16);
			curr_node = curr_node->next;
		}

		dll_list_add_sorted(curr_clique, &id, compare_uint16);

//		DFS on the current friend
		bool done = false;
		dfs_recursive(friends_graph, visited, &clique, curr_clique,
					  checked_users, friend, &done);
//		next
		friends_node = friends_node->next;
	}

	printf("The closest friend group of %s is:\n", name);

	dll_node_t *curr_node = dll_get_head(clique);
	for (size_t i = 0; i < clique->size; i++) {
		uint16_t friend = *(uint16_t *)curr_node->data;
		printf("%s\n", get_username(friend));
		curr_node = curr_node->next;
	}
}