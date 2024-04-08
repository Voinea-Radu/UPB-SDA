/*
 * Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
 */

#include <stdio.h>
#include "api/server.h"
#include "api/lru_cache.h"

#include "api/utils.h"

static response_t
*server_edit_document(server_t *s, char *doc_name, char *doc_content)
{
	/* TODO */
	return NULL;
}

static response_t
*server_get_document(server_t *s, char *doc_name)
{
	/* TODO */
	return NULL;
}

server_t *server_init(unsigned int cache_size)
{
	/* TODO */
	return NULL;
}

response_t *server_handle_request(server_t *server, request_t *req)
{
	/* TODO */
	return NULL;
}

void server_free(server_t **server)
{
	/* TODO */
}

void response_print(response_t *response)
{
	if (response) {
		printf(LOG_RESPONSE, response->server_id, response->server_response,
			   response->server_id, response->server_log);
		free(response->server_response);
		free(response->server_log);
		free(response);
	}
}
