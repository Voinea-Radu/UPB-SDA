//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include <stdint-gcc.h>
#include <string.h>
#include "post.h"
#include "../utils/utils.h"
#include "../utils/users.h"

// Private methods
// TODO docs
void __print_repost(post_t *post);

void __compute_post_ratio(linked_list_t *list, uint32_t *current_max,
						  uint32_t *current_max_id);

uint64_t __compute_common_repost(post_t *post, uint32_t repost_id_1,
								 uint32_t repost_id_2);

void free_post(post_t *post) {
	free(post->title);
	linked_list_free(post->likes);
	linked_list_free(post->reposts);
	free(post);
}

bool compare_post(void *data1, void *data2) {
	return ((post_t *)data1)->id == ((post_t *)data2)->id;
}

post_t *post_get(linked_list_t *posts, uint32_t post_id, uint32_t repost_id) {
	node_t *current = posts->head;

	while (current != NULL) {
		post_t *post = (post_t *)current->data;

		if (post->id == post_id) {
			if (repost_id == 0) {
				return post;
			}

			return post_get(post->reposts, repost_id, 0);
		}

		post_t *result = post_get(post->reposts, post_id, repost_id);
		if (result != NULL) {
			return result;
		}

		current = current->next;
	}

	return NULL;
}

post_t *
post_init(uint32_t id, uint16_t user_id, string_t title, bool is_repost) {
	post_t *post = safe_malloc(sizeof(post_t));

	post->id = id;
	post->user_id = user_id;
	post->title = title;

	post->is_repost = is_repost;

	post->likes = linked_list_init((void (*)(void *))free_int, compare_int);
	post->reposts = linked_list_init((void (*)(void *))free_post, compare_post);

	return post;
}

void posts_add_post(linked_list_t *posts, post_t *post) {
	static uint32_t next_post_id = 1;

	post->id = next_post_id++;
	linked_list_add(posts, post);
}

post_t *
posts_add_repost(linked_list_t *posts, uint32_t post_id, uint32_t repost_id,
				 uint32_t user_id) {
	post_t *post = post_get(posts, post_id, repost_id);

	if (post == NULL) {
		return NULL;
	}

	post_t *repost = post_init(0, user_id, strdup(post->title), true);
	posts_add_post(post->reposts, repost);

	return repost;
}

void __print_repost(post_t *post) {
	printf("Repost #%d by %s\n", post->id, get_username(post->user_id));
	for_each(post->reposts, (void (*)(void *))__print_repost);
}

void
post_print_reposts(linked_list_t *posts, uint32_t post_id, uint32_t repost_id) {
	post_t *post = post_get(posts, post_id, repost_id);

	if (post == NULL) {
		return;
	}

	if (post->is_repost) {
		printf("Repost #%d by %s\n", post->id, get_username(post->user_id));
	} else {
		printf("%s - Post by %s\n", post->title, get_username(post->user_id));
	}
	for_each(post->reposts, (void (*)(void *))__print_repost);
}

uint64_t __compute_common_repost(post_t *post, uint32_t repost_id_1,
								 uint32_t repost_id_2) {
	uint64_t result = 0;

	if (post->id == repost_id_1) {
		result = 0b10;
	}

	if (post->id == repost_id_2) {
		result = 0b01;
	}

	node_t *current = post->reposts->head;

	while (current != NULL) {
		post_t *current_post = (post_t *)current->data;

		result = result | __compute_common_repost(current_post, repost_id_1,
												  repost_id_2);

		if ((result & 0b11) == 0b11) {
			if (result > 0b11) {
				return result;
			}
			uint32_t output = post->id;
			output = output << 2;
			output = output | 0b11;

			return output;
		}

		current = current->next;
	}

	return result;
}

uint32_t post_get_common_repost(linked_list_t *list, uint32_t post_id,
								uint32_t repost_id_1, uint32_t repost_id_2) {
	post_t *post = post_get(list, post_id, 0);
	uint64_t output = __compute_common_repost(post, repost_id_1, repost_id_2);
	uint32_t common_repost_id = output >> 2;

	return common_repost_id;
}

void post_toggle_like(linked_list_t *posts, uint16_t user_id, uint32_t post_id,
					  uint32_t repost_id) {
	post_t *post = post_get(posts, post_id, repost_id);

	if (post == NULL) {
		return;
	}

	uint16_t *user_id_ptr = safe_malloc(sizeof(uint16_t));
	*user_id_ptr = user_id;

	if (linked_list_contains(post->likes, user_id_ptr)) {
		linked_list_remove(post->likes, user_id_ptr);
		if (post->is_repost) {
			printf("User %s unliked repost %s\n", get_username(user_id),
				   post->title);
		} else {
			printf("User %s unliked post %s\n", get_username(user_id),
				   post->title);
		}
		free(user_id_ptr);
		return;
	}

	linked_list_add(post->likes, user_id_ptr);
	if (post->is_repost) {
		printf("User %s liked repost %s\n", get_username(user_id), post->title);
		return;
	}
	printf("User %s liked post %s\n", get_username(user_id), post->title);
}

void __compute_post_ratio(linked_list_t *list, uint32_t *current_max,
						  uint32_t *current_max_id) {
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

		__compute_post_ratio(post->reposts, current_max, current_max_id);
		current = current->next;
	}
}

uint32_t post_get_ratio(linked_list_t *posts, uint32_t post_id) {
	post_t *post = post_get(posts, post_id, 0);

	uint32_t original_post_likes_count = post->likes->size;
	uint32_t reposts_likes_count = 0;
	uint32_t most_liked_repost_id = 0;

	__compute_post_ratio(post->reposts, &reposts_likes_count,
						 &most_liked_repost_id);

	if (original_post_likes_count >= reposts_likes_count) {
		return -1;
	}
	return most_liked_repost_id;
}

void post_delete(linked_list_t *posts, uint32_t post_id, uint32_t repost_id) {
	node_t *current = posts->head;

	while (current != NULL) {
		post_t *post = (post_t *)current->data;

		if (post->id == post_id) {
			if (repost_id == 0) {
				if (!post->is_repost) {
					printf("Deleted post %s\n", post->title);
				} else {
					printf("Deleted repost #%d of post %s\n", post->id,
						   post->title);
				}
				linked_list_remove(posts, post);
				return;
			}

			post_delete(post->reposts, repost_id, 0);
			return;
		}

		post_delete(post->reposts, post_id, repost_id);

		current = current->next;
	}
}

int cmp_post_recent(void *a, void *b) {
	post_t *post_a = *(post_t **)a;
	post_t *post_b = *(post_t **)b;

	return post_a->id - post_b->id;
}

void
posts_get_by_user_id(linked_list_t *posts, double_linked_list_t **curr_feed,
					 uint16_t user_id, uint16_t limit) {
	if (*curr_feed == NULL)
		*curr_feed = dll_list_init(sizeof(post_t), free_post);

	node_t *current = posts->head;
	while (current != NULL) {
		post_t *post = (post_t *)current->data;

		if (post->user_id == user_id) {
			dll_list_add_sorted_with_limit(*curr_feed, post, cmp_post_recent,
										   limit);
		}
		current = current->next;
	}
}