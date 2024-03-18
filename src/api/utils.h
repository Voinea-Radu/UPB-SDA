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

// Math utils

bool is_number(string_t string);

// Memory utils
void *safe_malloc(size_t size);

void *safe_realloc(void *ptr, size_t size);

void *safe_calloc(size_t size);

#endif //TEMA1_UTILS_H
