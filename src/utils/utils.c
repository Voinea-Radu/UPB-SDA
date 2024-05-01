//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include "utils.h"

uint uint_hash(uint value)
{
	value = ((value >> 16u) ^ value) * 0x45d9f3b;
	value = ((value >> 16u) ^ value) * 0x45d9f3b;
	value = (value >> 16u) ^ value;

	return value;
}

uint string_hash(string_t value)
{
	unsigned char *key_string = (unsigned char *)value;
	uint hash = 5381;
	int c;

	while ((c = *(key_string++))) {
		hash = ((hash << 5u) + hash) + c;
	}

	return hash;
}

bool string_equals(string_t string1, string_t string2)
{
	return strcmp(string1, string2) == 0;
}

string_t get_request_type_str(request_type_t request_type)
{
	switch (request_type) {
	case ADD_SERVER:
		return ADD_SERVER_REQUEST;
	case REMOVE_SERVER:
		return REMOVE_SERVER_REQUEST;
	case EDIT_DOCUMENT:
		return EDIT_REQUEST;
	case GET_DOCUMENT:
		return GET_REQUEST;
	}

	return NULL;
}

request_type_t get_request_type(string_t request_type_str)
{
	request_type_t type;

	if (!strncmp(request_type_str, ADD_SERVER_REQUEST,
				 strlen(ADD_SERVER_REQUEST)))
		type = ADD_SERVER;
	else if (!strncmp(request_type_str, REMOVE_SERVER_REQUEST,
					  strlen(REMOVE_SERVER_REQUEST)))
		type = REMOVE_SERVER;
	else if (!strncmp(request_type_str, EDIT_REQUEST, strlen(EDIT_REQUEST)))
		type = EDIT_DOCUMENT;
	else if (!strncmp(request_type_str, GET_REQUEST, strlen(GET_REQUEST)))
		type = GET_DOCUMENT;
	else
		exit_with_error("Unknown request type");

	return type;
}

void *safe_malloc(size_t size)
{
#if DEBUG
	void *result = safe_calloc(size);
#else  // DEBUG
	void *result = malloc(size);
#endif  // DEBUG
	check_or_exit(!result, MEMORY_ERROR);

	return result;
}

void *safe_calloc(size_t size)
{
	void *result = calloc(size, 1);

	check_or_exit(!result, MEMORY_ERROR);

	return result;
}

void *safe_realloc(void *ptr, size_t size)
{
	void *result = realloc(ptr, size);

	check_or_exit(!result, MEMORY_ERROR);

	return result;
}

void exit_with_error(string_t message)
{
	fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__);
	perror(message);
	exit(errno);
}

void check_or_exit(bool condition, string_t message)
{
	if (condition)
		exit_with_error(message);
}

int str_to_int(string_t str)
{
	return strtol(str, NULL, 10);
}

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


void *create_and_copy_explicit(void *src, uint size)
{
	void *dest = safe_malloc(size);
	memcpy(dest, src, size);
	return dest;
}

void *create_and_copy(void *src, uint (*get_size)(void *data))
{
	if (src == NULL)
		return NULL;
	return create_and_copy_explicit(src, get_size(src));
}

uint string_data_size(string_t data)
{
	return strlen(data) + 1;
}

void string_free(string_t *data)
{
	if (*data == NULL)
		return;

	free(*data);
	*data = NULL;
}

string_t server_queued(request_type_t request_type, string_t document_name)
{
	string_t request_type_str = get_request_type_str(request_type);
	string_t output = malloc(strlen(SERVER_QUEUED) + strlen(request_type_str) +
							 strlen(document_name) + 1);
	sprintf(output, SERVER_QUEUED, request_type_str, document_name);
	return output;
}

string_t database_entry_edited(string_t document_name)
{
	string_t output = malloc(strlen(DATABASE_ENTRY_EDITED) +
							 strlen(document_name) + 1);
	sprintf(output, DATABASE_ENTRY_EDITED, document_name);
	return output;
}

string_t database_entry_created(string_t document_name)
{
	string_t output = malloc(strlen(DATABASE_ENTRY_CREATED) +
							 strlen(document_name) + 1);
	sprintf(output, DATABASE_ENTRY_CREATED, document_name);
	return output;
}

string_t log_lazy_exec(uint task_queue_size)
{
	string_t output = malloc(strlen(LOG_LAZY_EXEC) + 10);
	sprintf(output, LOG_LAZY_EXEC, task_queue_size);
	return output;
}

string_t log_cache_hit(string_t document_name)
{
	string_t output = malloc(strlen(LOG_CACHE_HIT) + strlen(document_name) + 1);
	sprintf(output, LOG_CACHE_HIT, document_name);
	return output;
}

string_t log_cache_miss(string_t document_name)
{
	string_t output = malloc(strlen(LOG_CACHE_MISS) +
							 strlen(document_name) + 1);
	sprintf(output, LOG_CACHE_MISS, document_name);
	return output;
}

string_t log_cache_miss_with_evict(string_t document_name,
								   string_t evicted_document_name)
{
	string_t output = malloc(
			strlen(LOG_CACHE_MISS_WITH_EVICT) +
			strlen(document_name) + strlen(evicted_document_name) + 1);
	sprintf(output, LOG_CACHE_MISS_WITH_EVICT, document_name,
			evicted_document_name);
	return output;
}

string_t log_fault(string_t document_name)
{
	string_t output = malloc(strlen(LOG_FAULT) + strlen(document_name) + 1);
	sprintf(output, LOG_FAULT, document_name);
	return output;
}
