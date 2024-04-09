/*
 * Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
 */

#include <stdio.h>
#include "api/server.h"
#include "api/cache.h"

#include "api/utils.h"

static response_t *server_edit_document(server_t *server, document_t document)
{
	/* TODO */
	return NULL;
}

static response_t *server_get_document(server_t *server, document_t document)
{
	/* TODO */
	return NULL;
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

response_t *server_handle_request(server_t *server, request_t *request)
{
	switch (request->type) {
	case EDIT_DOCUMENT: {
		return server_edit_document(server, request->document);
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

#if DEBUG
	printf(LOG_RESPONSE, response->server_id, response->server_response,
		   response->server_id, response->server_log);
	free(response->server_response);
	free(response->server_log);
	free(response);
#endif // DEBUG
}
