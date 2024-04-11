/*
 * Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
 */

#ifndef UTILS_H
#define UTILS_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "constants.h"

void check_or_exit(bool condition, string_t message);

void exit_with_error(string_t message);

/**
 * @brief Should be used as hash function for server_t IDs,
 *      to find server_t's position on the hash ring
 */
unsigned int hash_uint(uint key);

/**
 * @brief Should be used as hash function for document names,
 *      to find the proper server_t on the hash ring
*/
unsigned int hash_string(string_t key);

string_t get_request_type_str(request_type_t request_type);

request_type_t get_request_type(string_t request_type_str);

/**
 * @brief Allocates memory for a buffer of size `size`. If the allocation fails,
 * 	the program exits with an error message.
 * @param size - size of the buffer
 * @return void* - pointer to the allocated memory
 */
void *safe_malloc(size_t size);

/**
 * @brief Reallocates memory for a buffer of size `size`. If the reallocation fails,
 * 	the program exits with an error message.
 * @param ptr - pointer to the buffer to be reallocated
 * @param size - size of the buffer
 * @return void* - pointer to the reallocated memory
 */
void *safe_realloc(void *ptr, size_t size);

/**
 * @brief Allocates memory for a buffer of size `size` and sets all the bytes to 0.
 * 	If the allocation fails, the program exits with an error message.
 * @param size - size of the buffer
 * @return void* - pointer to the allocated memory
 */
void *safe_calloc(size_t size);

int str_to_int(string_t str);

void read_quoted_string(string_t buffer, int buffer_len, int *start, int *end);

string_t increase_prefix(string_t prefix);

#endif /* UTILS_H */