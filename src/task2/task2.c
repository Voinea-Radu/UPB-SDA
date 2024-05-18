//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/users.h"
#include "task2.h"
#include "../generic/hash_map.h"
#include "../utils/utils.h"
#include "../utils/debug.h"
#include "database.h"

void print_post(post_t *post, string_t prefix)
{
	if (NULL == prefix) {
		prefix = strdup("");
		debug_log("Post %d: %s by %s\n", post->id, post->title, get_username(post->user_id));
	} else {
		debug_log("%sReposed by %s\n", prefix, get_username(post->user_id));
	}

	if (post->reposts->size != 0) {
		debug_log("%sReposts:\n", prefix);
		prefix = increase_prefix(prefix);
		linked_list_print_prefixed(post->reposts, prefix, (void (*)(void *, string_t))print_post);
	}
}

void handle_input_posts(char *input)
{
	static hash_map_t *commands_map = NULL;
	static database_t *database = NULL;

	if (NULL == commands_map) {
		commands_map = hash_map_init(10, (uint (*)(void *))string_hash, (bool (*)(void *, void *))string_equals);
		hash_map_put(commands_map, "create", handle_create);
		hash_map_put(commands_map, "repost", handle_repost);
		hash_map_put(commands_map, "common-repost", handle_common_repost);
		hash_map_put(commands_map, "like", handle_like);
		hash_map_put(commands_map, "ratio", handle_ratio);
		hash_map_put(commands_map, "delete", handle_delete);
		hash_map_put(commands_map, "get-likes", handle_get_reposts);
		hash_map_put(commands_map, "get-reposts", handle_get_likes);
	}

	if (NULL == database) {
		database = database_init();
	}

	string_t command = strdup(input);
	strtok(command, " ");

	if (NULL == command) {
		return;
	}

	void (*handler)(database_t *, string_t) = hash_map_get(commands_map, command);

	if (NULL == handler) {
		return;
	}

	int offset = strlen(command) + 1;

	command = strdup(input);
	command[strlen(command) - 1] = '\0';
	command += offset;
	handler(database, command);

#if DEBUG
	debug_log("\n");
	debug_log("Database: \n");
	linked_list_print_prefixed(database->posts, NULL, (void (*)(void *, string_t))print_post);
	debug_log("\n");
#endif
}

void handle_create(database_t *database, string_t command)
{
	string_t username = strtok(command, " ");
	string_t message = strtok(NULL, "\n");

	debug_log("Creating post: username:%s message:%s\n", username, message);

	post_t *post = post_init(0, get_user_id(username), message);

	database_add_post(database, post);
}

void handle_repost(database_t *database, string_t command)
{
	database = database;

	string_t username = strtok(command, " ");
	string_t post_id_str = strtok(NULL, " ");
	string_t repost_id_str = strtok(NULL, " ");

	debug_log("Creating repost: username:%s post_id:%s repost_id:%s\n", username, post_id_str, repost_id_str);

	uint32_t user_id = get_user_id(username);
	uint32_t post_id = strtol(post_id_str, NULL, 10);
	uint32_t repost_id = 0;

	if (NULL != repost_id_str) {
		repost_id = strtol(repost_id_str, NULL, 10);
	}

	database_add_repost(database, post_id, repost_id, user_id);
}

void handle_common_repost(database_t *database, string_t command)
{
	database = database;
	debug_log("%s\n", command);
}

void handle_like(database_t *database, string_t command)
{
	database = database;
	debug_log("%s\n", command);
}

void handle_ratio(database_t *database, string_t command)
{
	database = database;
	debug_log("%s", command);
}

void handle_delete(database_t *database, string_t command)
{
	database = database;
	debug_log("%s\n", command);
}

void handle_get_reposts(database_t *database, string_t command)
{
	database = database;
	debug_log("%s\n", command);
}

void handle_get_likes(database_t *database, string_t command)
{
	database = database;
	debug_log("%s\n", command);
}