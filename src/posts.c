//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/users.h"
#include "posts.h"

#include "generic/hash_map.h"
#include "utils/utils.h"
#include "utils/debug.h"

void handle_input_posts(char *input)
{
	static hash_map_t *commands_map = NULL;

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

	string_t command = strdup(input);
	strtok(command, " ");
	debug_log("[1] %s\n", command);

	if (NULL == command) {
		return;
	}

	void (*handler)(string_t) = hash_map_get(commands_map, command);

	if (NULL == handler) {
		return;
	}

	int offset = strlen(command) + 1;

	command = strdup(input);
	command[strlen(command) - 1] = '\0';
	command += offset;
	handler(command);
}

void handle_create(string_t command)
{
	debug_log("%s\n", command);
}

void handle_repost(string_t command)
{
	debug_log("%s\n", command);
}

void handle_common_repost(string_t command)
{
	debug_log("%s\n", command);
}

void handle_like(string_t command)
{
	debug_log("%s\n", command);
}

void handle_ratio(string_t command)
{
	debug_log("%s", command);
}

void handle_delete(string_t command)
{
	debug_log("%s\n", command);
}

void handle_get_reposts(string_t command)
{
	debug_log("%s\n", command);
}

void handle_get_likes(string_t command)
{
	debug_log("%s\n", command);
}