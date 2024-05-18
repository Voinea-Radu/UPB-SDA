//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef POSTS_H
#define POSTS_H

#include "../utils/utils.h"
#include "database.h"

void handle_input_posts(string_t input);

void handle_create(database_t *database, string_t command);

void handle_repost(database_t *database, string_t command);

void handle_common_repost(database_t *database, string_t command);

void handle_like(database_t *database, string_t command);

void handle_ratio(database_t *database, string_t command);

void handle_delete(database_t *database, string_t command);

void handle_get_reposts(database_t *database, string_t command);

void handle_get_likes(database_t *database, string_t command);

#endif // POSTS_H
