//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef UTILS_H
#define UTILS_H

#include <sys/types.h>
#include <stdint-gcc.h>
#include <stdbool.h>
#include "constants.h"

uint uint_hash(uint value);

uint string_hash(string_t string);

void *safe_malloc(size_t size);

void *safe_calloc(size_t size);

void *safe_realloc(void *ptr, size_t size);

void check_or_exit(bool condition, string_t message);

void exit_with_error(string_t message);

bool string_equals(string_t string1, string_t string2);

void free_int(uint16_t *data);

bool compare_int(void *data1, void *data2);

#endif // UTILS_H
