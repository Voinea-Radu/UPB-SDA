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

void posts_free_memory(linked_list_t *posts)
{
	if (posts != NULL) {
		linked_list_free(posts);
	}
}

linked_list_t *get_all_posts(){
	static linked_list_t *posts = NULL;

	if (NULL == posts) {
		posts = linked_list_init((void (*)(void *))free_post, compare_post);
	}

	return posts;
}

void handle_input_posts(const char* input)
{
	static hash_map_t *commands_map = NULL;
	string_t command = strdup(input);

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

	string_t result = strtok(command, " ");

	if (NULL == result) {
		return;
	}

	void (*handler)(linked_list_t *, string_t) = hash_map_get(commands_map, command);

	if (NULL == handler) {
		return;
	}

	string_t arguments = strtok(NULL, "\n");

	handler(get_all_posts(), arguments);

#if DEBUG
	debug_log("\n");
	debug_log("Database: \n");
	linked_list_print_prefixed(posts->posts, "", (void (*)(string_t, void *))print_post);
	debug_log("\n");
#endif
}

void handle_create(linked_list_t *posts, string_t command)
{
	string_t username = strtok(command, " ");
	string_t message = strdup(strtok(NULL, "\n"));

#if DEBUG
	debug_log("Creating post: username: %s | message: %s\n", username, message);
#endif

	post_t *post = post_init(0, get_user_id(username), message, false);
	posts_add_post(posts, post);

	printf("Created %s for %s\n", message, username); // TODO move string to constants
}

void handle_repost(linked_list_t *posts, string_t command)
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

	post_t *post = posts_add_repost(posts, post_id, repost_id, user_id);
	printf("Created repost #%d for %s\n", post->id, username); // TODO move string to constants
}

void handle_get_reposts(linked_list_t *posts, string_t command)
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

	post_print_reposts(posts, post_id, repost_id);
}

void handle_common_repost(linked_list_t *posts, string_t command)
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

	uint32_t common_repost_id = post_get_common_repost(posts, post_id, repost_id_1, repost_id_2);

	printf("The first common repost of %d and %d is %d\n", repost_id_1, repost_id_2, common_repost_id); // TODO move string to constants
}

void handle_like(linked_list_t *posts, string_t command)
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

	post_toggle_like(posts, user_id, post_id, repost_id);
}

void handle_get_likes(linked_list_t *posts, string_t command)
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

	post_t *post = post_get(posts, post_id, repost_id);

	if (repost_id != 0) {
		printf("Repost #%d has %d likes\n", post->id, post->likes->size);
	} else {
		printf("Post %s has %d likes\n", post->title, post->likes->size);
	}
}

void handle_ratio(linked_list_t *posts, string_t command)
{
	string_t post_id_str = strtok(command, " ");

	uint32_t post_id = strtol(post_id_str, NULL, 10);

#if DEBUG
	debug_log("Getting ratio: post_id: %d\n", post_id);
#endif

	uint32_t ratio_post_id = post_get_ratio(posts, post_id);

	if (ratio_post_id == 0xffffffff) {
		printf("The original post is the highest rated\n");
		return;
	}

	printf("Post %d got ratio'd by repost %d\n", post_id, ratio_post_id);
}

void handle_delete(linked_list_t *posts, string_t command)
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

	post_delete(posts, post_id, repost_id);
}
