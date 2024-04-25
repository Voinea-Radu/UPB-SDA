//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include <stdio.h>
#include "server.h"
#include "cache.h"

#include "../utils/utils.h"
#include "../utils/debug.h"

server_t *server_init(uint cache_size, uint server_id)
{
	server_t *server = safe_malloc(sizeof(server_t));

	server->server_id = server_id;
	server->hash = uint_hash(server_id);

	server->database = database_init(DATABASE_HASH_TABLE_SIZE);
	server->cache = cache_init(cache_size);
	server->task_queue = queue_init((bool (*)(void *, void *))request_equal, (uint (*)(void *))request_size, (void (*)(void **))request_free);

	return server;
}

static response_t *server_edit_document_immediate(server_t *server, document_t *document, bool bypass_cache)
{
	if (bypass_cache) {
		database_put(server->database, document);

		return response_init(server->server_id,
							 log_cache_miss(document->name),
							 database_entry_created(document->name));
	}

#if DEBUG
	debug_log("Editing document %s\n", document->name);

	if(strcmp("public_economic.txt", document->name) == 0) {
		debug_log("Editing public_economic.txt\n");
		server_print(server, "");
	}

#endif // DEBUG
	string_t lookup_result = cache_get(server->cache, document->name);

	if (lookup_result != NULL) {
		document_t *evicted_document = cache_put(server->cache, document);

		string_free(&lookup_result);
		document_free(&evicted_document);

		return response_init(server->server_id,
							 log_cache_hit(document->name),
							 database_entry_edited(document->name));
	}

	lookup_result = database_get(server->database, document->name);

	if (lookup_result != NULL) {
		string_free(&lookup_result);
		document_t *evicted_document = cache_put(server->cache, document);

		if (evicted_document != NULL) {
			database_put(server->database, evicted_document);

			response_t *response = response_init(server->server_id,
												 log_cache_miss_with_evict(document->name, evicted_document->name),
												 database_entry_edited(document->name));

			document_free(&evicted_document);

			return response;
		}

		return response_init(server->server_id,
							 log_cache_miss(document->name),
							 database_entry_edited(document->name));
	}

	document_t *evicted_document = cache_put(server->cache, document);

	if (evicted_document != NULL) {
		database_put(server->database, evicted_document);

		response_t *response = response_init(server->server_id,
											 log_cache_miss_with_evict(document->name, evicted_document->name),
											 database_entry_created(document->name));

		document_free(&evicted_document);

		return response;
	}

	return response_init(server->server_id,
						 log_cache_miss(document->name),
						 database_entry_created(document->name));
}

static response_t *server_edit_document(server_t *server, document_t *document, bool execute_immediately, bool bypass_cache)
{
	if (execute_immediately) {
		response_t *output = server_edit_document_immediate(server, document, bypass_cache);

#if DEBUG
		if(strcmp("public_economic.txt", document->name) == 0) {
			debug_log("Edited public_economic.txt\n");
			server_print(server, "");
		}

#endif // DEBUG

		return output;
	}

	request_t *request = request_init(EDIT_DOCUMENT, document_init(document->name, document->content));
	queue_enqueue(server->task_queue, request);

	free(request);

	return response_init(server->server_id,
						 log_lazy_exec(server->task_queue->size),
						 server_queued(EDIT_DOCUMENT, document->name));
}

static response_t *server_get_document(server_t *server, document_t *document)
{
	execute_task_queue(server);

#if DEBUG
	debug_log("Getting document %s\n", document->name);
#endif // DEBUG

	string_t lookup_result = cache_get(server->cache, document->name);

	if (lookup_result != NULL) {
		return response_init(server->server_id, log_cache_hit(document->name), lookup_result);
	}

	lookup_result = database_get(server->database, document->name);

	if (lookup_result == NULL) {
		return response_init(server->server_id, log_fault(document->name), NULL);
	}

	document_t *evicted_document = cache_put_explicit(server->cache, document->name, lookup_result);

	if (evicted_document != NULL) {
		database_put(server->database, evicted_document);

		response_t *response = response_init(server->server_id, log_cache_miss_with_evict(document->name, evicted_document->name), lookup_result);

		document_free(&evicted_document);

		return response;
	}

	return response_init(server->server_id, log_cache_miss(document->name), lookup_result);
}

response_t *server_handle_request(server_t *server, request_t *request, bool execute_immediately, bool bypass_cache)
{
#if DEBUG
	debug_log("Request received by server %u\n", server->server_id);
#endif // DEBUG
	response_t *output = NULL;

	switch (request->type) {
	case EDIT_DOCUMENT: {
		output = server_edit_document(server, request->document, execute_immediately, bypass_cache);
		break;
	}
	case GET_DOCUMENT: {
		output = server_get_document(server, request->document);
		break;
	}
	default:
		return NULL;
	}
	return output;
}

void server_free(server_t **server)
{
	cache_free(&(*server)->cache);
	database_free(&(*server)->database);
	queue_free(&(*server)->task_queue);

	free(*server);
	*server = NULL;
}

void execute_task_queue(server_t *server)
{
#if DEBUG
	debug_log("Executing task queue:\n");
	queue_print(server->task_queue, (string_t (*)(void *))queued_task_to_string, "\t", true);
#endif // DEBUG

	while (!queue_is_empty(server->task_queue)) {
		request_t *request = queue_dequeue(server->task_queue);
		response_t *response = server_handle_request(server, request, true, false);

		response_print(response);

		request_free(&request);
		response_free(&response);
	}
}

uint server_size(server_t *server)
{
	return sizeof(*server);
}

bool server_compare(server_t *server1, server_t *server2)
{
	return server1->hash == server2->hash;
}

document_t **server_get_all_documents(server_t *server, uint *size)
{
	*size = 0;
	*size += server->cache->data->size;
	*size += server->database->data->size;

	document_t **documents = safe_malloc(*size * sizeof(document_t *));
	uint index = 0;

	document_t **database_entries = hash_map_get_values(server->database->data);
	document_t **cache_entries = hash_map_get_values(server->cache->data);

	for (uint i = 0; i < server->database->data->size; i++) {
		documents[index++] = database_entries[i];
	}

	for (uint i = 0; i < server->cache->data->size; i++) {
		documents[index++] = cache_entries[i];
	}

	free(database_entries);
	free(cache_entries);

	return documents;
}

void remove_document(server_t *server, document_t *document)
{
#if DEBUG
	debug_log("[Server %d] Removing document %s\n",server->server_id, document->name);
#endif // DEBUG
	cache_remove(server->cache, document->name);
	database_remove(server->database, document->name);
}
