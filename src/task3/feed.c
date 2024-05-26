#include <string.h>

#include "feed.h"
#include "../utils/users.h"
#include "../utils/constants.h"
#include "../friends.h"
#include "../task2/task2.h"
#include "../task2/post.h"

void __print_all_posts_for_user(linked_list_t *posts, uint16_t user_id);

void print_profile(string_t username);
void __print_reposting_friends(linked_list_t *posts, linked_list_t *friends);
void friends_repost(string_t username, uint32_t post_id);

void show_feed(graph_t *friends_graph, char *name, int size);

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
	} else if (!strcmp(command, "common-groups")) {
		// TODO
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
		curr_node = curr_node->next;
	}
}

void __print_all_posts_for_user(linked_list_t *posts, uint16_t user_id) {
	node_t *current = posts->head;

	while (current != NULL) {
		post_t *post = (post_t *)current->data;

		if (post->user_id == user_id) {
			if (post->is_repost) {
				printf("Reposted: %s\n", post->title);
			} else {
				printf("Posted: %s\n", post->title);
			}
		}

		__print_all_posts_for_user(post->reposts, user_id);
		current = current->next;
	}
}

void print_profile(string_t username) {
	uint16_t id = get_user_id(username);
	__print_all_posts_for_user(get_all_posts(), id);
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

