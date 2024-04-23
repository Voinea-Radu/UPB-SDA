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

	server->database = database_init(DATABASE_HASH_TABLE_SIZE);
	server->cache = cache_init(cache_size);
	server->task_queue = queue_init((bool (*)(void *, void *))request_equal, (uint (*)(void *))request_size, (void (*)(void **))request_free);

	return server;
}

static response_t *server_edit_document_immediate(server_t *server, document_t *document)
{
#if DEBUG
	debug_log("Editing document %s\n", document->name);
#endif // DEBUG

	string_t output = cache_get(server->cache, document->name);

	if (output == NULL) {
		if (database_get(server->database, document->name) == NULL) {
			document_t *evicted_document = cache_put(server->cache, document);

			if (evicted_document != NULL) {
				database_put(server->database, *evicted_document);

				return response_init(server->server_id,
									 log_cache_miss_with_evict(document->name, evicted_document->name),
									 database_entry_created(document->name));
			}

			return response_init(server->server_id,
								 log_cache_miss(document->name),
								 database_entry_created(document->name));
		}

		document_t *evicted_document = cache_put(server->cache, document);

		if (evicted_document != NULL) {
			database_put(server->database, *evicted_document);

			return response_init(server->server_id,
								 log_cache_miss_with_evict(document->name, evicted_document->name),
								 database_entry_edited(document->name));
		}

		return response_init(server->server_id,
							 log_cache_miss(document->name),
							 database_entry_edited(document->name));

	}

	string_free(&output);

	cache_put(server->cache, document);

	return response_init(server->server_id,
						 log_cache_hit(document->name),
						 database_entry_edited(document->name));
}

static response_t *server_edit_document(server_t *server, document_t *document, bool execute_immediately)
{
	if (execute_immediately) {
		return server_edit_document_immediate(server, document);
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

	string_t output = cache_get(server->cache, document->name);

	if (output != NULL) {
		return response_init(server->server_id, log_cache_hit(document->name), output);
	}

	output = database_get(server->database, document->name);

	if (output == NULL) {
		return response_init(server->server_id, log_fault(document->name), NULL);
	}

	document->content = output;

	document_t *evicted_document = cache_put(server->cache, document);

	if (evicted_document != NULL) {
		database_put(server->database, *evicted_document);
		return response_init(server->server_id, log_cache_miss_with_evict(document->name, evicted_document->name), output);
	}

	return response_init(server->server_id, log_cache_miss(document->name), output);
}

response_t *server_handle_request(server_t *server, request_t *request, bool execute_immediately)
{
	response_t *output = NULL;

	switch (request->type) {
	case EDIT_DOCUMENT: {
		output = server_edit_document(server, request->document, execute_immediately);
#if DEBUG
		server_print(server, "");
#endif // DEBUG
		break;
	}
	case GET_DOCUMENT: {
		output = server_get_document(server, request->document);
#if DEBUG
		server_print(server, "");
#endif // DEBUG
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
		response_t *response = server_handle_request(server, request, true);

		response_print(response);

		request_free(&request);
		response_free(&response);
	}
}
