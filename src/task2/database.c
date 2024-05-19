//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include <stddef.h>
#include <string.h>
#include "database.h"
#include "../utils/utils.h"
#include "../utils/users.h"

database_t *database_init()
{
	database_t *database = safe_malloc(sizeof(database_t));

	database->next_post_id = 1;
	database->posts = linked_list_init();

	return database;
}

void database_add_post(database_t *database, post_t *post)
{
	post->id = database->next_post_id++;
	linked_list_add(database->posts, post);
}

void database_add_repost_to_repost(post_t *post, uint32_t original_repost_id, post_t *repost)
{
	node_t *current = post->reposts->head;

	while (current != NULL) {
		post_t *current_post = (post_t *)current->data;

		if (current_post->id == original_repost_id) {
			linked_list_add(current_post->reposts, repost);
			return;
		}

		database_add_repost_to_repost(current_post, original_repost_id, repost);

		current = current->next;
	}
}

void database_add_repost(database_t *database, uint32_t original_post_id, uint32_t original_repost_id, uint32_t user_id)
{
	node_t *current = database->posts->head;

	while (current != NULL) {
		post_t *post = (post_t *)current->data;

		if (post->id == original_post_id) {
			post_t *repost = safe_malloc(sizeof(post_t));

			repost->id = database->next_post_id++;
			repost->user_id = user_id;
			repost->title = post->title;
			repost->likes = linked_list_init();
			repost->reposts = linked_list_init();

			if (original_repost_id == 0) {
				linked_list_add(post->reposts, repost);
				return;
			}

			database_add_repost_to_repost(post, original_repost_id, repost);
			return;
		}

		current = current->next;
	}
}

void __print_repost(post_t *post)
{
	printf("Repost #%d by %s\n", post->id, get_username(post->user_id));

	for_each(post->reposts, (void (*)(void *))__print_repost);
}

void __print_reposts(post_t *post)
{
	printf("%s - Post by %s\n", post->title, get_username(post->user_id));

	for_each(post->reposts, (void (*)(void *))__print_repost);
}

void __print_reposts_of_reposts(post_t *post)
{
	printf("Repost #%d by %s\n", post->id, get_username(post->user_id));

	for_each(post->reposts, (void (*)(void *))__print_repost);
}

bool __iterate_posts(linked_list_t *post_list, uint32_t post_id, void (*callback)(post_t *))
{
	node_t *current = post_list->head;

	while (current != NULL) {
		post_t *post = (post_t *)current->data;

		if (post->id == post_id) {
			callback(post);
			return true;
		}

		bool result = __iterate_posts(post->reposts, post_id, callback);

		if (result) {
			return true;
		}

		current = current->next;
	}
	return false;
}

void print_reposts(database_t *database, uint32_t post_id, uint32_t repost_id)
{
	node_t *current = database->posts->head;

	while (current != NULL) {
		post_t *post = (post_t *)current->data;

		if (post->id == post_id) {
			if (repost_id == 0) {
				__print_reposts(post);
				return;
			}

			__iterate_posts(database->posts, repost_id, (void (*)(post_t *))__print_reposts_of_reposts);
			return;
		}

		current = current->next;
	}
}



