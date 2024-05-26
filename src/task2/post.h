//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

// TODO Add docs

#ifndef POST_H
#define POST_H

#include <stdint-gcc.h>
#include "../utils/constants.h"
#include "../generic/linked_list.h"
#include "../task1/double_linked_list.h"

typedef struct {
	uint32_t id;
	uint16_t user_id;
	string_t title;

	bool is_repost;

	linked_list_t *likes;    // list<int> / list<user_id>
	linked_list_t *reposts; // list<post_t>
} post_t;

post_t *
post_init(uint32_t id, uint16_t user_id, string_t title, bool is_repost);

void free_post(post_t *post);

bool compare_post(void *data1, void *data2);

void posts_add_post(linked_list_t *posts, post_t *post);

post_t *
posts_add_repost(linked_list_t *posts, uint32_t post_id, uint32_t repost_id,
				 uint32_t user_id);

void
post_print_reposts(linked_list_t *posts, uint32_t post_id, uint32_t repost_id);

uint32_t post_get_common_repost(linked_list_t *list, uint32_t post_id,
								uint32_t repost_id_1, uint32_t repost_id_2);

post_t *post_get(linked_list_t *posts, uint32_t post_id, uint32_t repost_id);

void post_toggle_like(linked_list_t *posts, uint16_t user_id, uint32_t post_id,
					  uint32_t repost_id);

uint32_t post_get_ratio(linked_list_t *posts, uint32_t post_id);

void post_delete(linked_list_t *posts, uint32_t post_id, uint32_t repost_id);

double_linked_list_t *
posts_get_by_user_id(linked_list_t *posts, double_linked_list_t *friends,
					 uint16_t user_id, uint16_t limit);

#endif // POST_H
