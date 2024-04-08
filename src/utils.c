/*
 * Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
 */

#include "api/utils.h"

unsigned int hash_uint(uint key)
{
	key = ((key >> 16u) ^ key) * 0x45d9f3b;
	key = ((key >> 16u) ^ key) * 0x45d9f3b;
	key = (key >> 16u) ^ key;

	return key;
}

unsigned int hash_string(string_t key)
{
	unsigned char *key_string = (unsigned char *)key;
	unsigned int hash = 5381;
	int c;

	while ((c = *key_string++))
		hash = ((hash << 5u) + hash) + c;

	return hash;
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

	if (!strncmp(request_type_str, ADD_SERVER_REQUEST, strlen(ADD_SERVER_REQUEST)))
		type = ADD_SERVER;
	else if (!strncmp(request_type_str, REMOVE_SERVER_REQUEST, strlen(REMOVE_SERVER_REQUEST)))
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
	void *result = malloc(size);

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
	fprintf(stderr, "%s\n", message);
	perror(message);
	exit(errno);
}

void check_or_exit(bool condition, string_t message)
{
	if (condition)
		exit_with_error(message);
}
