//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef DATABASE_H
#define DATABASE_H

#include "post.h"
#include "../generic/linked_list.h"

typedef struct {
	uint32_t next_post_id;
	linked_list_t *posts; // list<post_t>
} database_t;

database_t *database_init();

void database_add_post(database_t *database, post_t *post);

void database_add_repost_to_repost(post_t *post, uint32_t original_repost_id, post_t *repost);

/**
 * @param database 				The database
 * @param original_post_id 		The original post id
 * @param original_repost_id 	The target repost id. You can set it to 0 if you want to add a repost to the original post.
 * @param user_id 				The user id
 */
void database_add_repost(database_t *database, uint32_t original_post_id, uint32_t original_repost_id, uint32_t user_id);

string_t increase_prefix(string_t prefix);

#endif // DATABASE_H
