/*
Nume: Voinea Radu-Mihai
Grupa: 315 CA
*/

#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include "api/string_utils.h"

void *safe_malloc(size_t size)
{
	void *result = malloc(size);

	if (0 == result) {
		printf("There was an error while allocating memory!\nExiting...\n");
		exit(1);
	}

	return result;
}

void *safe_calloc(size_t size)
{
	void *result = calloc(size, 1);

	if (0 == result) {
		printf("There was an error while allocating memory!\nExiting...\n");
		exit(1);
	}

	return result;
}

void *safe_realloc(void *ptr, size_t size)
{
	void *result = realloc(ptr, size);

	if (0 == result) {
		printf("There was an error while allocating memory!\nExiting...\n");
		exit(1);
	}

	return result;
}

bool is_number(string_t string)
{
	for (size_t i = 0; i < strlen(string); i++)
		if (!isdigit(string[i]) && string[i] != '-')
			return false;

	return true;
}

/**
 * This function exists for the sole reason that CLion debugger on windows is not printing any output unless fflush is called
 */
int printf(const char *format, ...)
{
	int output;

	__builtin_va_list argv;
	__builtin_va_start(argv, format);

	output = vfprintf(stdout, format, argv);

	__builtin_va_end(argv);
	fflush(stdout);

	return output;
}
