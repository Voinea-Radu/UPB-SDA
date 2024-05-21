//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef POSTS_H
#define POSTS_H

#include "../utils/utils.h"
#include "../generic/linked_list.h"
void posts_free_memory(linked_list_t *posts);

linked_list_t *get_all_posts();

void handle_input_posts(const char* input);

void handle_create(linked_list_t *posts, string_t command);

void handle_repost(linked_list_t *posts, string_t command);

void handle_common_repost(linked_list_t *posts, string_t command);

void handle_like(linked_list_t *posts, string_t command);

void handle_ratio(linked_list_t *posts, string_t command);

void handle_delete(linked_list_t *posts, string_t command);

void handle_get_reposts(linked_list_t *posts, string_t command);

void handle_get_likes(linked_list_t *posts, string_t command);

#endif // POSTS_H
