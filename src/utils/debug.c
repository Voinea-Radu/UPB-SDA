//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include "debug.h"
#include "../backend/load_balancer.h"


#if DEBUG

string_t key_to_string(void *key)
{
	return (string_t)key;
}

void cache_print_entry(string_t prefix, string_t key, string_t value)
{
	debug_log("%s- Key: %s, Value: %s\n", prefix, key, value);
}

void cache_print(cache_t *cache, string_t prefix)
{
	string_t new_prefix = increase_prefix(prefix);

	debug_log("%sCache capacity: %d\n", prefix, cache->capacity);
	debug_log("%sCache size: %d\n", prefix, cache->map->size);

	debug_log("%sCache entries:\n", prefix);

	hash_map_print(cache->map, new_prefix,
				   (void (*)(string_t, void *, void *))cache_print_entry);

	debug_log("%sCache history:\n", prefix);
	queue_print(cache->queue, key_to_string, new_prefix, false);

	free(new_prefix);
}

void load_balancer_print(load_balancer_t *load_balancer)
{
	debug_log("Servers count: %d\n", load_balancer->servers_count);
	for (uint i = 0; i < load_balancer->servers_count; i++) {
		server_print(load_balancer->servers[i], "");
	}
}

string_t queued_task_to_string(request_t *request)
{
	string_t output = safe_malloc(sizeof(char) * 10000);
	sprintf(output, "Request type: %s, Document name: %s, Document content: %s", get_request_type_str(request->type),
			request->document->name, request->document->content);
	return output;
}

void server_print(server_t *server, string_t prefix)
{
	string_t __new_prefix = increase_prefix(prefix);
	string_t new_prefix = increase_prefix(__new_prefix);

	debug_log("%sServer with id %d:\n", prefix, server->server_id);

	debug_log("\t%sDatabase:\n", prefix);
	database_print(server->database, new_prefix);

	debug_log("\t%sCache:\n", prefix);
	cache_print(server->cache, new_prefix);

	debug_log("\t%sTask queue:\n", prefix);
	queue_print(server->task_queue, (string_t (*)(void *))queued_task_to_string, new_prefix, true);

	free(__new_prefix);
	free(new_prefix);
}

void database_print_entry(string_t prefix, string_t key, string_t value)
{
	debug_log("%s- Key: %s, Value: %s\n", prefix, key, value);
}

void database_print(database_t *database, string_t prefix)
{
	hash_map_print(database->data, prefix, (void (*)(string_t, void *, void *))database_print_entry);
}

void queue_print(queue_t *queue, string_t (*to_string_function)(void *), string_t prefix, bool should_free)
{
	queue_node_t *node = queue->head;

	while (node) {
		string_t data_string = to_string_function(node->data);
		debug_log("%s%s\n", prefix, data_string);

		if (should_free) {
			free(data_string);
		}

		node = node->next;
	}
}

#endif
