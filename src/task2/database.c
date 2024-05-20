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
	database->posts = linked_list_init(free_post, compare_post);

	return database;
}

void database_add_post(database_t *database, post_t *post)
{
	post->id = database->next_post_id++;
	linked_list_add(database->posts, post);
}

void __add_repost_to_repost(post_t *post, uint32_t original_repost_id, post_t *repost)
{
	node_t *current = post->reposts->head;

	while (current != NULL) {
		post_t *current_post = (post_t *)current->data;

		if (current_post->id == original_repost_id) {
			linked_list_add(current_post->reposts, repost);
			return;
		}

		__add_repost_to_repost(current_post, original_repost_id, repost);

		current = current->next;
	}
}

void database_add_repost(database_t *database, uint32_t original_post_id, uint32_t original_repost_id, uint32_t user_id)
{
	node_t *current = database->posts->head;

	while (current != NULL) {
		post_t *post = (post_t *)current->data;

		if (post->id == original_post_id) {
			post_t *repost = post_init(database->next_post_id++, user_id, strdup(post->title));

			if (original_repost_id == 0) {
				linked_list_add(post->reposts, repost);
				return;
			}

			__add_repost_to_repost(post, original_repost_id, repost);
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

void database_print_reposts(database_t *database, uint32_t post_id, uint32_t repost_id)
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

uint32_t __print_common_repost(post_t *post, uint32_t repost_id1, uint32_t repost_id2)
{
	uint32_t result = 0;

	if (post->id == repost_id1) {
		result = 0b10;
	}

	if (post->id == repost_id2) {
		result = 0b01;
	}

	node_t *current = post->reposts->head;

	while (current != NULL) {
		post_t *current_post = (post_t *)current->data;

		result = result | __print_common_repost(current_post, repost_id1, repost_id2);

		if (result == 0b111) {
			return 0b111;
		}

		if (result == 0b11) {
			printf("The first common repost of %d and %d is %d\n", repost_id1, repost_id2, post->id);
			return 0b111;
		}

		current = current->next;
	}

	return result;
}

bool __get_common_reposts(linked_list_t *list, uint32_t post_id, uint32_t repost_id1, uint32_t repost_id2)
{
	node_t *current = list->head;

	while (current != NULL) {
		post_t *post = (post_t *)current->data;

		if (post->id == post_id) {
			__print_common_repost(post, repost_id1, repost_id2);
			return true;
		}

		bool result = __get_common_reposts(post->reposts, post_id, repost_id1, repost_id2);

		if (result) {
			return true;
		}

		current = current->next;
	}
	return false;
}

void database_get_common_reposts(database_t *database, uint32_t post_id, uint32_t repost_id1, uint32_t repost_id2)
{
	__get_common_reposts(database->posts, post_id, repost_id1, repost_id2);
}

void __toggle_like(linked_list_t *list, uint16_t __user_id, uint32_t post_id, uint32_t repost_id, bool is_repost)
{
	node_t *current = list->head;

	while (current != NULL) {
		post_t *post = (post_t *)current->data;

		if (post->id == post_id) {
			if (repost_id == 0) {
				uint32_t *user_id = safe_malloc(sizeof(uint32_t));
				*user_id = __user_id;

				if (linked_list_contains(post->likes, user_id)) {
					if (is_repost) {
						printf("User %s unliked repost %s\n", get_username(__user_id), post->title);
					} else {
						printf("User %s unliked post %s\n", get_username(__user_id), post->title);
					}
					linked_list_remove(post->likes, user_id);
					return;
				}
				if (is_repost) {
					printf("User %s liked repost %s\n", get_username(__user_id), post->title);
				} else {
					printf("User %s liked post %s\n", get_username(__user_id), post->title);
				}

				linked_list_add(post->likes, user_id);
				return;
			}

			__toggle_like(post->reposts, __user_id, repost_id, 0, true);
			return;
		}

		__toggle_like(post->reposts, __user_id, post_id, repost_id, is_repost);

		current = current->next;
	}
}

void database_toggle_like(database_t *database, uint16_t user_id, uint32_t post_id, uint32_t repost_id)
{
	__toggle_like(database->posts, user_id, post_id, repost_id, false);
}

void __get_like_count(linked_list_t *list, uint32_t post_id, uint32_t repost_id, bool is_repost)
{
	node_t *current = list->head;
	while (current != NULL) {
		post_t *post = (post_t *)current->data;
		if (post->id == post_id) {
			if (repost_id == 0) {
				if (is_repost) {
					printf("Repost #%d has %d likes\n", post->id, post->likes->size);
				} else {
					printf("Post %s has %d likes\n", post->title, post->likes->size);
				}
				return;
			}
			__get_like_count(post->reposts, repost_id, 0, true);
			return;
		}
		__get_like_count(post->reposts, post_id, repost_id, is_repost);
		current = current->next;
	}
}


void database_get_like_count(database_t *database, uint32_t post_id, uint32_t repost_id)
{
	__get_like_count(database->posts, post_id, repost_id, false);
}

void __get_ratio(linked_list_t *list, uint32_t *current_max, uint32_t *current_max_id)
{
	node_t *current = list->head;
	while (current != NULL) {
		post_t *post = (post_t *)current->data;
		uint32_t likes_count = post->likes->size;

		if (likes_count > *current_max) {
			*current_max = likes_count;
			*current_max_id = post->id;
		}

		if (likes_count == *current_max) {
			if (post->id < *current_max_id) {
				*current_max_id = post->id;
			}
		}

		__get_ratio(post->reposts, current_max, current_max_id);
		current = current->next;
	}
}

void database_get_ratio(database_t *database, uint32_t post_id)
{
	node_t *current = database->posts->head;
	while (current != NULL) {
		post_t *post = (post_t *)current->data;
		if (post->id == post_id) {
			uint32_t original_post_likes_count = post->likes->size;
			uint32_t reposts_likes_count = 0;
			uint32_t most_liked_repost_id = 0;

			__get_ratio(post->reposts, &reposts_likes_count, &most_liked_repost_id);

			if (original_post_likes_count >= reposts_likes_count) {
				printf("The original post is the highest rated\n");
			} else {
				printf("Post %d got ratio'd by repost %d\n", post->id, most_liked_repost_id);
			}

			return;
		}
		current = current->next;
	}
}

void __delete_post(linked_list_t *list, uint32_t post_id, uint32_t repost_id, bool is_repost)
{
	node_t *current = list->head;

	while (current != NULL) {
		post_t *post = (post_t *)current->data;

		if (post->id == post_id) {
			if (repost_id == 0) {
				if(is_repost) {
					printf("Deleted repost #%d of post %s\n", post->id, post->title);
				} else {
					printf("Deleted post %s\n", post->title);
				}
				linked_list_remove(list, post);
				return;
			}

			__delete_post(post->reposts, repost_id, 0, true);
			return;
		}

		__delete_post(post->reposts, post_id, repost_id, is_repost);

		current = current->next;
	}
}

void database_delete(database_t *database, uint32_t post_id, uint32_t repost_id)
{
	__delete_post(database->posts, post_id, repost_id, false);
}