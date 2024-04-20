/*
 * Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
 */

#include <stdio.h>
#include "api/state_manager.h"
#include "api/constants.h"
#include "api/utils.h"
#include "api/load_balancer.h"

void init(FILE *input)
{
	char buffer[REQUEST_LENGTH + 1];

	string_t line = fgets(buffer, REQUEST_LENGTH + 1, input);

	check_or_exit(line == NULL, "Empty input file");

	int requests_num = str_to_int(buffer);
	bool enable_vnodes = strstr(buffer, "ENABLE_VNODES");

	load_balancer_t *load_balancer = load_balancer_init(enable_vnodes);

	for (int i = 0; i < requests_num; i++) {
		process_next_request(input, load_balancer);
	}

#if DEBUG
	load_balancer_print(load_balancer);
#endif // DEBUG

	load_balancer_free(&load_balancer);

}

void process_next_request(FILE *input, load_balancer_t *load_balancer)
{
	char buffer[REQUEST_LENGTH + 1];

	string_t line = fgets(buffer, REQUEST_LENGTH + 1, input);

	check_or_exit(line == NULL, "Insufficient requests");

	request_type_t request_type = get_request_type(buffer);

	switch (request_type) {
	case ADD_SERVER: {
		handle_add_server(buffer, load_balancer);
		break;
	}
	case REMOVE_SERVER: {
		handle_remove_server(buffer, load_balancer);
		break;
	}
	case EDIT_DOCUMENT: {
		handle_edit_document(buffer, load_balancer);
		break;
	}
	case GET_DOCUMENT: {
		handle_get_document(buffer, load_balancer);
		break;
	}
	}
}

void handle_add_server(string_t buffer, load_balancer_t *load_balancer)
{
	int server_id = str_to_int(buffer + strlen(ADD_SERVER_REQUEST) + 1);
	int cache_size = str_to_int(strchr(buffer + strlen(ADD_SERVER_REQUEST) + 1, ' '));

	check_or_exit(cache_size < 0, "Cache size must be positive.");

	load_balancer_add_server(load_balancer, server_id, cache_size);

#if DEBUG
	printf("[DEBUG] Added server %d with cache size %d\n", server_id, cache_size);
#endif // DEBUG
}

void handle_remove_server(string_t buffer, load_balancer_t *load_balancer)
{
	int server_id = str_to_int(buffer + strlen(REMOVE_SERVER_REQUEST) + 1);

	load_balancer_remove_server(load_balancer, server_id);
#if DEBUG
	printf("Deleted server %d", server_id);
#endif // DEBUG
}

void handle_edit_document(string_t buffer, load_balancer_t *load_balancer)
{
	string_t document_name = safe_calloc(DOC_NAME_LENGTH + 1);
	string_t document_content = safe_calloc(DOC_CONTENT_LENGTH + 1);

	int word_start = -1;
	int word_end = -1;

	read_quoted_string(buffer, REQUEST_LENGTH, &word_start, &word_end);
	memcpy(document_name, buffer + word_start + 1, word_end - word_start - 1);

	char *tmp_buffer = buffer + word_end + 1;

	word_start = -1;
	read_quoted_string(tmp_buffer, DOC_CONTENT_LENGTH, &word_start, &word_end);
	memcpy(document_content, tmp_buffer + word_start + 1, word_end - word_start - 1);

	while (word_end == -1) {
		read_quoted_string(buffer, DOC_CONTENT_LENGTH, &word_start, &word_end);
		memcpy(document_content + strlen(document_content), buffer, word_end == -1 ? strlen(buffer) : (unsigned)word_end);
	}

	request_t server_request = {
			.type = EDIT_DOCUMENT,
			.document={
					.name = document_name,
					.content = document_content
			}
	};

	response_t *response = load_balancer_forward_request(load_balancer, &server_request);

	//free(server_request.document.name);
	//free(server_request.document.content);

	response_print(response);

#if DEBUG
	printf("[DEBUG] Edited document %s with content %s\n\n", document_name, document_content);
#endif // DEBUG
}

void handle_get_document(string_t buffer, load_balancer_t *load_balancer)
{
	string_t document_name = safe_calloc(DOC_NAME_LENGTH + 1);

	int word_start = -1;
	int word_end = -1;

	read_quoted_string(buffer, REQUEST_LENGTH, &word_start, &word_end);
	memcpy(document_name, buffer + word_start + 1, word_end - word_start - 1);

	request_t server_request = {
			.type = GET_DOCUMENT,
			.document = {
					.name = document_name
			}
	};

	response_t *response = load_balancer_forward_request(load_balancer, &server_request);

	free(server_request.document.name);

	response_print(response);
#if DEBUG
	printf("Got document %s\n\n", document_name);
#endif // DEBUG
}
