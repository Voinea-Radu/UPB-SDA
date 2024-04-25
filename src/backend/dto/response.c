//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include "response.h"
#include "../../utils/utils.h"

response_t *response_init(string_t server_log, string_t server_response)
{
	response_t *response = safe_malloc(sizeof(response_t));
	response->server_id = 0xffffffff;
	response->server_log = server_log;
	response->server_response = server_response;
	return response;
}

void response_free(response_t **response)
{
	if (!response || !*response) {
		return;
	}

	string_free(&(*response)->server_log);
	string_free(&(*response)->server_response);
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
