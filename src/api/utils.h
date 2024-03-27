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

// Math utils

typedef unsigned char u8;
typedef signed char s8;
typedef long s64;

s64 min(s64 a, s64 b);

s64 max(s64 a, s64 b);

s64 clamp(s64 value, s64 min, s64 max);

s64 power_of_two(s64 value);

// Memory utils
void *safe_malloc(size_t size);

void *safe_realloc(void *ptr, size_t size);

void *safe_calloc(size_t size);

#endif //TEMA1_UTILS_H
