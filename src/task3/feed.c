#include <string.h>

#include "feed.h"
#include "../utils/users.h"
#include "../utils/constants.h"
#include "../friends.h"
#include "../task2/task2.h"
#include "../task2/post.h"

void __print_all_posts_for_user(linked_list_t *posts, uint16_t user_id);

void print_profile(string_t username);

void handle_input_feed(char *input)
{
	char *command = strtok(input, "\n ");

	if (!command) {
		return;
	}

	if (!strcmp(command, "feed")) {
		// TODO
	} else if (!strcmp(command, "view-profile")) {
		string_t username = strtok(NULL, "\n ");

		print_profile(username);
	} else if (!strcmp(command, "friends-repost")) {
		// TODO
	} else if (!strcmp(command, "common-groups")) {
		// TODO
	}
}

void __print_all_posts_for_user(linked_list_t *posts, uint16_t user_id)
{
	node_t *current = posts->head;

	while (current != NULL) {
		post_t *post = (post_t *)current->data;

		if (post->user_id == user_id) {
			if (post->is_repost) {
				printf("Reposted: %s\n", post->title);
			}else{
				printf("Posted: %s\n", post->title);
			}
		}

		__print_all_posts_for_user(post->reposts, user_id);
		current = current->next;
	}
}

void print_profile(string_t username)
{
	uint16_t id = get_user_id(username);
	__print_all_posts_for_user(get_all_posts(), id);
}

