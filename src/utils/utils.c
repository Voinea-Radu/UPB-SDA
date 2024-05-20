//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "utils.h"

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

void check_or_exit(bool condition, string_t message)
{
	if (condition)
		exit_with_error(message);
}

void exit_with_error(string_t message)
{
	fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__);
	perror(message);
	exit(errno);
}

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

void free_int(__attribute__((unused)) void *data)
{

}

bool compare_int(void *data1, void *data2)
{
	int data1_int = *(int *)data1;
	int data2_int = *(int *)data2;
	return data1_int == data2_int;
}