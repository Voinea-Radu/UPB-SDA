//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef POSTS_H
#define POSTS_H

#include "utils/utils.h"

typedef struct post_t{
	int id;
	int user_id;
	string_t title;

	// likes
	int likes_count;
	int* likes;

	struct post_t* reports;
}post_t;

void handle_input_posts(string_t input);

void handle_create(string_t command);

void handle_repost(string_t command);

void handle_common_repost(string_t command);

void handle_like(string_t command);

void handle_ratio(string_t command);

void handle_delete(string_t command);

void handle_get_reposts(string_t command);

void handle_get_likes(string_t command);

#endif // POSTS_H
