//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include <stdio.h>
#include "server.h"
#include "cache.h"

#include "../utils/utils.h"

request_t *request_init(request_type_t type, document_t document)
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

static response_t *server_edit_document(server_t *server, document_t document, bool execute_immediately)
{
	if (execute_immediately) {
		document_t *output = cache_get(server->cache, document.name);

		if (output == NULL) {
			output = database_get(server->database, document.name);

			if (output == NULL) {
				void* evicted_key = malloc(sizeof(void*));
				cache_put(server->cache, document.name, document.content, &evicted_key);

				// TODO handle evicted key

				return response_init(server->server_id,
									 log_cache_miss(document.name),
									 database_entry_created(document.name));
			}

			void* evicted_key = malloc(sizeof(void*));
			cache_put(server->cache, document.name, output, &evicted_key);
			// TODO handle evicted key

			return response_init(server->server_id,
								 log_cache_miss(document.name),
								 database_entry_edited(document.name));

		}

		void* evicted_key = malloc(sizeof(void*));
		cache_put(server->cache, document.name, output, &evicted_key);
		// TODO handle evicted key

		return response_init(server->server_id,
							 log_cache_hit(document.name),
							 database_entry_edited(document.name));
	}

	request_t *request = request_init(EDIT_DOCUMENT, document);
	queue_enqueue(server->task_queue, request);

	return response_init(server->server_id,
						 log_lazy_exec(server->task_queue->size),
						 server_queued(EDIT_DOCUMENT, document.name));
}

static response_t *server_get_document(server_t *server, document_t document)
{
	execute_task_queue(server);

	document_t *output = hash_map_get(server->cache->map, document.name);

	if (output != NULL) {
		return response_init(server->server_id, log_cache_hit(document.name), output->content);
	}

	output = hash_map_get(server->database, document.name);

	if (output == NULL) {
		return response_init(server->server_id, log_fault(document.name), NULL);
	}

	return response_init(server->server_id, log_cache_miss(document.name), output->content);
}

server_t *server_init(uint cache_size, uint server_id)
{
	server_t *server = safe_malloc(sizeof(server_t));

	server->server_id = server_id;

	server->database = hash_map_init(DATABASE_HASH_TABLE_SIZE, (uint (*)(void *))hash_string);
	server->cache = cache_init(cache_size, (uint (*)(void *))hash_string);
	server->task_queue = queue_init();

	return server;
}

response_t *server_handle_request(server_t *server, request_t *request, bool execute_immediately)
{
	switch (request->type) {
	case EDIT_DOCUMENT: {
		return server_edit_document(server, request->document, execute_immediately);
	}
	case GET_DOCUMENT: {
		return server_get_document(server, request->document);
	}
	default:
		return NULL;
	}
}

void server_free(server_t **server)
{
	cache_free(&(*server)->cache);

	free(*server);
	*server = NULL;
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
	while (!queue_is_empty(server->task_queue)) {
		request_t *request = queue_dequeue(server->task_queue);
		response_t *response = server_handle_request(server, request, true);

		response_print(response);
	}
}

string_t queued_task_to_string(request_t *request)
{
	string_t output = safe_malloc(sizeof(char) * 10000);
	sprintf(output, "Request type: %s, Document name: %s, Document content: %s", get_request_type_str(request->type),
			request->document.name, request->document.content);
	return output;

}

void server_print(server_t *server, string_t prefix)
{
	string_t new_prefix = increase_prefix(prefix);
	new_prefix = increase_prefix(new_prefix);

	printf("%sServer with id %d:\n", prefix, server->server_id);

	printf("\t%sDatabase:\n", prefix);
	database_print(server->database, new_prefix);

	printf("\t%sCache:\n", prefix);
	cache_print(server->cache, new_prefix);

	printf("\t%sTask queue:\n", prefix);
	queue_print(server->task_queue, (string_t (*)(void *))queued_task_to_string, new_prefix);
}

uint hash_document(document_t *document)
{
	return hash_string(document->name);
}
