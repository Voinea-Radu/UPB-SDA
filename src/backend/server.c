//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include <stdio.h>
#include "server.h"
#include "cache.h"

#include "../utils/utils.h"

request_t *request_init(request_type_t type, document_t *document)
{
	request_t *request = safe_malloc(sizeof(request_t));
	request->type = type;
	request->document = document;
	return request;
}

response_t *response_init(uint server_id, string_t server_log, string_t server_response)
{
	response_t *response = safe_malloc(sizeof(response_t));
	response->server_id = server_id;
	response->server_log = server_log;
	response->server_response = server_response;
	return response;
}

static response_t *server_edit_document_immediate(server_t *server, document_t *document)
{
#if DEBUG
	debug_log("Editing document %s\n", document->name);
#endif // DEBUG

	string_t output = cache_get(server->cache, document->name);

	if (output == NULL) {
		output = database_get(server->database, document->name);

		if (output == NULL) {
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

server_t *server_init(uint cache_size, uint server_id)
{
	server_t *server = safe_malloc(sizeof(server_t));

	server->server_id = server_id;

	server->database = database_init(DATABASE_HASH_TABLE_SIZE);
	server->cache = cache_init(cache_size);
	server->task_queue = queue_init((bool (*)(void *, void *))request_equal, (uint (*)(void *))request_size, (void (*)(void **))request_free);

	return server;
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

void response_free(response_t **response)
{
	free((*response)->server_log);
	free((*response)->server_response);
	free(*response);

	*response = NULL;
}

void response_print(response_t *response)
{
	if (!response) {
		return;
	}

	printf(LOG_RESPONSE, response->server_id, response->server_response,
		   response->server_id, response->server_log);
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
	}
}

#if DEBUG

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

#endif // DEBUG

uint document_hash(document_t *document)
{
	return string_hash(document->name);
}

bool request_equal(request_t *request1, request_t *request2)
{
	return request1->type == request2->type && strcmp(request1->document->name, request2->document->name) == 0;
}

uint request_size(request_t *request)
{
	return sizeof(request_t);
}

void request_free(request_t **request)
{
	document_free(&(*request)->document);

	free(*request);
	*request = NULL;
}
