/*
 * Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
 */

#include <stdio.h>
#include "api/server.h"
#include "api/cache.h"

#include "api/utils.h"

static response_t *server_edit_document(server_t *server, document_t document, bool execute_imeediately)
{
	if (execute_imeediately) {
		document_t *output = hash_map_get(server->database, document.name);

		if (output == NULL) {
			hash_map_put(server->database, document.name, &document);
		} else {
			output->content = document.content;
		}

		return NULL;
	}

	request_t *request = safe_malloc(sizeof(request_t));
	request->type = EDIT_DOCUMENT;
	request->document = document;

	queue_enqueue(server->task_queue, request);

	response_t *response = malloc(sizeof(response_t));

	response->server_id = server->server_id;
	response->server_response = SERVER_QUEUED;
	response->server_log = LOG_LAZY_EXEC;

	return response;
}

static response_t *server_get_document(server_t *server, document_t document)
{
	execute_task_queue(server);

	response_t *response = safe_malloc(sizeof(response_t));
	document_t *output = hash_map_get(server->cache->map, document.name);

	if (output != NULL) {
		response->server_response = output->content;
		response->server_log = LOG_CACHE_HIT;

		return response;
	}

	output = hash_map_get(server->database, document.name);

	if (output == NULL) {
		response->server_response = NULL;
		response->server_log = LOG_FAULT;

		return response;
	}

	response->server_response = output->content;
	response->server_log = LOG_CACHE_MISS;

	return response;
}

server_t *server_init(uint cache_size, uint server_id)
{
	server_t *server = safe_malloc(sizeof(server_t));

	server->server_id = server_id;

	server->database = hash_map_init(DATABASE_HASH_TABLE_SIZE);
	server->cache = cache_init(cache_size);
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
	free(response->server_response);
	free(response->server_log);
	free(response);
}

void execute_task_queue(server_t *server)
{
	while (!queue_is_empty(server->task_queue)) {
		request_t *request = queue_dequeue(server->task_queue);
		response_t *response = server_handle_request(server, request, true);

		response_print(response);
	}
}

void server_print(server_t *server, string_t prefix)
{
	string_t new_prefix = increase_prefix(prefix);
	new_prefix = increase_prefix(new_prefix);

	printf("%sServer with id %d:\n",prefix, server->server_id);

	printf("\t%sDatabase:\n", prefix);
	hash_map_print(server->database, new_prefix);

	printf("\t%sCache:\n", prefix);
	cache_print(server->cache, new_prefix);

	printf("\t%sTask queue:\n", prefix);
	queue_print(server->task_queue, new_prefix);
}
