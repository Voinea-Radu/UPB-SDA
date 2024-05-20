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
		hash_map_put(commands_map, "get-likes", handle_get_likes);
		hash_map_put(commands_map, "get-reposts", handle_get_reposts);
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

	if (command[strlen(command) - 1] == '\n') {
		command[strlen(command) - 1] = '\0';
	}
	command += offset;
	handler(database, command);

#if DEBUG
	debug_log("\n");
	debug_log("Database: \n");
	linked_list_print_prefixed(database->posts, "", (void (*)(string_t, void *))print_post);
	debug_log("\n");
#endif
}

void handle_create(database_t *database, string_t command)
{
	string_t username = strtok(command, " ");
	string_t message = strtok(NULL, "\n");

#if DEBUG
	debug_log("Creating post: username: %s | message: %s\n", username, message);
#endif

	post_t *post = post_init(0, get_user_id(username), message);

	database_add_post(database, post);
	printf("Created %s for %s\n", message, username);
}

void handle_repost(database_t *database, string_t command)
{
	string_t username = strtok(command, " ");
	string_t post_id_str = strtok(NULL, " ");
	string_t repost_id_str = strtok(NULL, " ");

	uint32_t user_id = get_user_id(username);
	uint32_t post_id = strtol(post_id_str, NULL, 10);
	uint32_t repost_id = 0;

	if (NULL != repost_id_str) {
		repost_id = strtol(repost_id_str, NULL, 10);
	}

#if DEBUG
	debug_log("Creating repost: username: %s | post_id: %d | repost_id: %d\n", username, post_id, repost_id);
#endif

	database_add_repost(database, post_id, repost_id, user_id);
	printf("Created repost #%d for %s\n", database->next_post_id - 1, username);
}

void handle_get_reposts(database_t *database, string_t command)
{
	string_t post_id_str = strtok(command, " ");
	string_t repost_id_str = strtok(NULL, " ");

	uint32_t post_id = strtol(post_id_str, NULL, 10);
	uint32_t repost_id = 0;

	if (NULL != repost_id_str) {
		repost_id = strtol(repost_id_str, NULL, 10);
	}

#if DEBUG
	debug_log("Printing reposts for post with id %d and repost: %d\n", post_id, repost_id);
#endif

	database_print_reposts(database, post_id, repost_id);
}

void handle_common_repost(database_t *database, string_t command)
{
	string_t post_id_str = strtok(command, " ");
	string_t repost_id_1_str = strtok(NULL, " ");
	string_t repost_id_2_str = strtok(NULL, " ");

	uint32_t post_id = strtol(post_id_str, NULL, 10);
	uint32_t repost_id_1 = strtol(repost_id_1_str, NULL, 10);
	uint32_t repost_id_2 = strtol(repost_id_2_str, NULL, 10);

#if DEBUG
	debug_log("%s\n", command);
#endif

	database_get_common_reposts(database, post_id, repost_id_1, repost_id_2);
}

void handle_like(database_t *database, string_t command)
{
	string_t username = strtok(command, " ");
	string_t post_id_str = strtok(NULL, " ");
	string_t repost_id_str = strtok(NULL, " ");

	uint32_t user_id = get_user_id(username);
	uint32_t post_id = strtol(post_id_str, NULL, 10);
	uint32_t repost_id = 0;

	if (NULL != repost_id_str) {
		repost_id = strtol(repost_id_str, NULL, 10);
	}

#if DEBUG
	debug_log("Creating like: username: %s | post_id: %d | repost_id: %d\n", username, post_id, repost_id);
#endif

	database_toggle_like(database, user_id, post_id, repost_id);
}


void handle_get_likes(database_t *database, string_t command)
{
	string_t post_id_str = strtok(command, " ");
	string_t repost_id_str = strtok(NULL, " ");

	uint32_t post_id = strtol(post_id_str, NULL, 10);
	uint32_t repost_id = 0;

	if (NULL != repost_id_str) {
		repost_id = strtol(repost_id_str, NULL, 10);
	}

#if DEBUG
	debug_log("Getting likes: post_id: %d | repost_id: %d\n", post_id, repost_id);
#endif

	database_get_like_count(database, post_id, repost_id);
}

void handle_ratio(database_t *database, string_t command)
{
	string_t post_id_str = strtok(command, " ");

	uint32_t post_id = strtol(post_id_str, NULL, 10);

#if DEBUG
	debug_log("Getting ratio: post_id: %d\n", post_id);
#endif

	database_get_ratio(database, post_id);

}

void handle_delete(database_t *database, string_t command)
{
	string_t post_id_str = strtok(command, " ");
	string_t repost_id_str = strtok(NULL, " ");

	uint32_t post_id = strtol(post_id_str, NULL, 10);
	uint32_t repost_id = 0;

	if (NULL != repost_id_str) {
		repost_id = strtol(repost_id_str, NULL, 10);
	}

#if DEBUG
	debug_log("Deleting post: post_id: %d | repost_id: %d\n", post_id, repost_id);
#endif

	database_delete(database, post_id, repost_id);
}
