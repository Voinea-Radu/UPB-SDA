/*
Nume: Voinea Radu-Mihai
Grupa: 315 CA
*/

#ifndef TEMA1_UTILS_H
#define TEMA1_UTILS_H

#include <stddef.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdbool.h>

#include "string_utils.h"
#include "linked_list.h"

// Math utils

#define max(a, b) ((a) > (b) ? (a) : (b))

#define min(a, b) ((a) < (b) ? (a) : (b))

int64_t clamp(int64_t value, int64_t min, int64_t max);

// Memory utils
void *safe_malloc(size_t size);

void *safe_realloc(void *ptr, size_t size);

void *safe_calloc(size_t size);

#endif //TEMA1_UTILS_H
