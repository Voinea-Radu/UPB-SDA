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
	debug_log("Adding server %d with cache size %u and hash %u\n", server_id, cache_size, uint_hash(server_id));
#endif // DEBUG
	server_t *server = server_init(cache_size, server_id);
	node_t *current_node = load_balancer->servers->head;
	server_t *current_server = NULL;

	int index = 0;
	bool found = false;

	while (current_node) {
		current_server = current_node->data;
		if (server->hash < current_server->hash) {
			found = true;
			break;
		}

		current_node = current_node->next;
		index++;
	}

	if (load_balancer->servers->head != NULL) {
		if (current_server == NULL || !found) {
			current_server = load_balancer->servers->head->data;
		}
	}

	linked_list_add_at_index(load_balancer->servers, server, index);
	load_balancer->servers_count++;

	free(server);

	if (current_server != NULL) {
		execute_task_queue(current_server);

		uint current_server_documents_count = 0;
		document_t **current_server_documents = server_get_all_documents(current_server, &current_server_documents_count);

		for (uint i = 0; i < current_server_documents_count; i++) {
			document_t *document = current_server_documents[i];
			server_t *target_server = get_target_server(load_balancer, document);

			if (target_server->server_id == current_server->server_id) {
#if DEBUG
				debug_log("Document %s already on correct server %d\n", document->name, current_server->server_id);
#endif // DEBUG
				document_free(&document);
				continue;
			}

#if DEBUG
			debug_log("Migrating document %s from server %d to server %d\n", document->name, current_server->server_id, target_server->server_id);
#endif // DEBUG

			remove_document(current_server, document);
			request_t *request = request_init(EDIT_DOCUMENT, document);
			response_t *response = load_balancer_forward_request(load_balancer, request, true, true);

			request_free(&request);
			response_free(&response);
		}

		free(current_server_documents);
	}
}

void load_balancer_remove_server(load_balancer_t *load_balancer, uint server_id)
{
#if DEBUG
	debug_log("Removing server %d\n", server_id);
#endif // DEBUG
	server_t *server_to_remove = server_init(0, server_id);
	server_t *removed_server = linked_list_remove(load_balancer->servers, server_to_remove);

	execute_task_queue(removed_server);

	uint removed_server_documents_count = 0;
	document_t **removed_server_documents = server_get_all_documents(removed_server, &removed_server_documents_count);

	for (uint i = 0; i < removed_server_documents_count; i++) {
		server_t *target_server = get_target_server(load_balancer, removed_server_documents[i]);

#if DEBUG
		debug_log("Migrating document %s from (deleted) server %d to server %d\n", removed_server_documents[i]->name, removed_server->server_id,
				  target_server->server_id);
#endif // DEBUG

		request_t *request = request_init(EDIT_DOCUMENT, removed_server_documents[i]);
		response_t *response = load_balancer_forward_request(load_balancer, request, true, true);

		request_free(&request);
		response_free(&response);
	}

	free(removed_server_documents);

	server_free(&server_to_remove);
	server_free(&removed_server);
}

server_t *get_target_server(load_balancer_t *load_balancer, document_t *document)
{
	uint hash = load_balancer->hash_document(document);
	node_t *current_node = load_balancer->servers->head;

	while (current_node) {
		server_t *current_server = current_node->data;

		if (hash <= current_server->hash) {
			return current_server;
		}

		current_node = current_node->next;
	}

	return load_balancer->servers->head->data;
}

response_t *load_balancer_forward_request(load_balancer_t *load_balancer, request_t *request, bool execute_immediately, bool bypass_cache)
{
	server_t *target_server = get_target_server(load_balancer, request->document);
	return server_handle_request(target_server, request, execute_immediately, bypass_cache);
}

void load_balancer_free(load_balancer_t **load_balancer)
{
	linked_list_free(&(*load_balancer)->servers);

	free(*load_balancer);
	*load_balancer = NULL;
}

