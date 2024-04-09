/*
 * Copyright (c) 2024, Andrei Otetea <andreiotetea23@gmail.com>
 * Copyright (c) 2024, Eduard Marin <marin.eduard.c@gmail.com>
 * Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api/load_balancer.h"
#include "api/cache.h"
#include "api/utils.h"
#include "api/constants.h"

void read_quoted_string(string_t buffer, int buffer_len, int *start, int *end)
{
	*end = -1;

	for (int i = 0; i < buffer_len && buffer[i] != '\0'; ++i) {
		if (buffer[i] != '"')
			continue;

		if (*start == -1) {
			*start = i;
		} else {
			*end = i;
			break;
		}
	}
}

request_type_t read_request_arguments(FILE *input_file, string_t buffer,
									  int *maybe_server_id, int *maybe_cache_size,
									  char **maybe_doc_name, char **maybe_doc_content)
{
	request_type_t req_type;
	int word_start = -1;
	int word_end = -1;

	check_or_exit(fgets(buffer, REQUEST_LENGTH + 1, input_file) == NULL, "Insufficient requests");

	req_type = get_request_type(buffer);

	if (req_type == ADD_SERVER) {
		*maybe_server_id = atoi(buffer + strlen(ADD_SERVER_REQUEST) + 1);
		*maybe_cache_size = atoi(strchr(
				buffer + strlen(ADD_SERVER_REQUEST) + 1, ' '));
	} else if (req_type == REMOVE_SERVER) {
		*maybe_server_id = atoi(buffer + strlen(REMOVE_SERVER_REQUEST) + 1);
	} else {
		*maybe_doc_name = safe_calloc(DOC_NAME_LENGTH + 1);

		read_quoted_string(buffer, REQUEST_LENGTH, &word_start, &word_end);
		memcpy(*maybe_doc_name, buffer + word_start + 1,
			   word_end - word_start - 1);

		if (req_type == EDIT_DOCUMENT) {
			char *tmp_buffer = buffer + word_end + 1;

			*maybe_doc_content = safe_calloc(DOC_CONTENT_LENGTH + 1);

			/* Read the content, which might be a multiline quoted string */
			word_start = -1;
			read_quoted_string(tmp_buffer, DOC_CONTENT_LENGTH,
							   &word_start, &word_end);

			if (word_end == -1)
				strcpy(*maybe_doc_content, tmp_buffer + word_start + 1);
			else
				strncpy(*maybe_doc_content, tmp_buffer + word_start + 1,
						word_end - word_start - 1);

			while (word_end == -1) {
				check_or_exit(fgets(buffer, DOC_CONTENT_LENGTH + 1, input_file) == NULL, "Document content is not properly quoted.");

				read_quoted_string(buffer, DOC_CONTENT_LENGTH,
								   &word_start, &word_end);
				memcpy(*maybe_doc_content + strlen(*maybe_doc_content), buffer,
					   word_end == -1 ? strlen(buffer) : (unsigned)word_end);
			}
		} else {
			*maybe_doc_content = NULL;
		}
	}

	return req_type;
}

void apply_requests(FILE *input_file, char *buffer,
					int requests_num, bool enable_vnodes)
{
	char *doc_name, *doc_content;
	int server_id, cache_size;

	load_balancer_t *main = load_balancer_init(enable_vnodes);

	for (int i = 0; i < requests_num; i++) {
		request_type_t req_type = read_request_arguments(input_file, buffer,
														 &server_id, &cache_size, &doc_name, &doc_content);

		if (req_type == ADD_SERVER) {
			check_or_exit(cache_size < 0, "Cache size must be positive.");
			load_balancer_add_server(main, server_id, cache_size);
		} else if (req_type == REMOVE_SERVER) {
			load_balancer_remove_server(main, server_id);
		} else {
			request_t server_request = {
					.type = req_type,
					.document_name = doc_name,
			};

			if (req_type == EDIT_DOCUMENT) {
				server_request.document_content = doc_content;
			}

			response_t *response = load_balancer_forward_request(main, &server_request);

			free(server_request.document_name);
			free(server_request.document_content);

			response_print(response);
		}
	}

	load_balancer_free(&main);
}

int main(int argc, string_t *argv)
{
	FILE *input;
	int requests_num;
	bool enable_vnodes;

	char buffer[REQUEST_LENGTH + 1];

	if (argc < 2) {
		printf("Usage: %s <input_file>\n", argv[0]);
		return -1;
	}

	input = fopen(argv[1], "rt");
	check_or_exit(input == NULL, "Missing input file");

	string_t line = fgets(buffer, REQUEST_LENGTH + 1, input);

	check_or_exit(line == NULL, "Empty input file");
	requests_num = atoi(buffer);
	enable_vnodes = strstr(buffer, "ENABLE_VNODES");

	apply_requests(input, buffer, requests_num, enable_vnodes);

	fclose(input);

	return 0;
}
