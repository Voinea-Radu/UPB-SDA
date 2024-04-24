//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include "load_balancer.h"
#include "server.h"
#include "../utils/utils.h"

load_balancer_t *load_balancer_init(bool enable_vnodes)
{
	load_balancer_t *load_balancer = safe_malloc(sizeof(load_balancer_t));

	load_balancer->enable_vnodes = enable_vnodes;
	load_balancer->hash_document = document_hash;
	load_balancer->servers_count = 0;

	load_balancer->servers = linked_list_init((uint (*)(void *))server_size, (bool (*)(void *, void *))server_compare,
											  (void (*)(void **))server_free);

	return load_balancer;
}

void load_balancer_add_server(load_balancer_t *load_balancer, int server_id, int cache_size)
{
#if DEBUG
	debug_log("Adding server %d with cache size %d\n", server_id, cache_size);
#endif // DEBUG
	server_t *server = server_init(cache_size, server_id);
	node_t *current_node = load_balancer->servers->head;

	int index = 0;

	while (current_node) {
		server_t *current_server = current_node->data;
		if (server->hash < current_server->hash) {
			break;
		}

		current_node = current_node->next;
		index++;
	}

	linked_list_add_at_index(load_balancer->servers, server, index);
	load_balancer->servers_count++;

	free(server);

#if DEBUG
	load_balancer_print(load_balancer);
	debug_log("\n");
#endif // DEBUG
}

void load_balancer_remove_server(load_balancer_t *load_balancer, uint server_id)
{
#if DEBUG
	debug_log("Removing server %d\n", server_id);
#endif // DEBUG
	server_t *server_to_remove = server_init(0, server_id);
	server_t *removed_server = linked_list_remove(load_balancer->servers, server_to_remove);

	execute_task_queue(removed_server);

	document_t **cache_entries = hash_map_get_values(removed_server->cache->data);
	document_t **database_entries = hash_map_get_values(removed_server->database->data);

	for (uint i = 0; i < removed_server->cache->data->size; i++){
		request_t *request = request_init(EDIT_DOCUMENT, cache_entries[i]);
		response_t *response = load_balancer_forward_request(load_balancer, request, true, true);

		request_free(&request);
		response_free(&response);
	}

	for (uint i = 0; i < removed_server->database->data->size; i++){
		request_t *request = request_init(EDIT_DOCUMENT, database_entries[i]);
		response_t *response = load_balancer_forward_request(load_balancer, request, true, true);

		request_free(&request);
		response_free(&response);
	}

	free(cache_entries);
	free(database_entries);

	server_free(&server_to_remove);
	server_free(&removed_server);
#if DEBUG
	load_balancer_print(load_balancer);
	debug_log("\n");
#endif // DEBUG
}

response_t *load_balancer_forward_request(load_balancer_t *load_balancer, request_t *request, bool execute_immediately, bool bypass_cache)
{
	uint hash = load_balancer->hash_document(request->document);

#if DEBUG
	debug_log("Forwarding request with hash %u\n", hash);
#endif // DEBUG

	node_t *current_node = load_balancer->servers->head;

	while (current_node) {
		server_t *current_server = current_node->data;

		if (hash <= current_server->hash) {
			return server_handle_request(current_server, request, execute_immediately, bypass_cache);
		}

		current_node = current_node->next;
	}

	return server_handle_request(load_balancer->servers->head->data, request, execute_immediately, bypass_cache);
}

void load_balancer_free(load_balancer_t **load_balancer)
{
	linked_list_free(&(*load_balancer)->servers);

	free(*load_balancer);
	*load_balancer = NULL;
}

