/*
Nume: Voinea Radu-Mihai
Grupa: 315 CA
*/

#include <stdio.h>
#include <stdlib.h>

#include "api/utils.h"

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

/**
 * This function exists for the sole reason that CLion debugger on windows is
 * not printing any output unless fflush is called
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

s64 clamp(s64 value, s64 min, s64 max)
{
	if (value < min)
		return min;

	if (value > max)
		return max;

	return value;
}

s64 power_of_two(s64 value)
{
	return 1 << value;
}

s64 min(s64 a, s64 b)
{
	return a < b ? a : b;
}

s64 max(s64 a, s64 b)
{
	return a > b ? a : b;
}

