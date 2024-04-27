//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef UTILS_H
#define UTILS_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "constants.h"

// ==================== Hashing ====================
/**
 * @brief Hashes a string using the djb2 algorithm
 *
 * @param value	The string to be hashed
 * @return uint The hash value
*/
uint string_hash(string_t value);

/**
 * @brief Hashes an unsigned integer
 *
 * @param value	The integer to be hashed
 * @return uint The hash value
*/
uint uint_hash(uint value);

// ==================== Other ====================

void check_or_exit(bool condition, string_t message);

void exit_with_error(string_t message);

bool string_equals(string_t string1, string_t string2);


string_t get_request_type_str(request_type_t request_type);

request_type_t get_request_type(string_t request_type_str);

void *safe_malloc(size_t size);

void *safe_realloc(void *ptr, size_t size);

void *safe_calloc(size_t size);

int str_to_int(string_t str);

void read_quoted_string(string_t buffer, int buffer_len, int *start, int *end);

string_t increase_prefix(string_t prefix);

string_t server_queued(request_type_t request_type, string_t document_name);

string_t database_entry_edited(string_t document_name);

string_t database_entry_created(string_t document_name);

string_t log_lazy_exec(uint task_queue_size);

string_t log_cache_hit(string_t document_name);

string_t log_cache_miss(string_t document_name);

string_t log_cache_miss_with_evict(string_t document_name,
								   string_t evicted_document_name);

string_t log_fault(string_t document_name);

void *create_and_copy_explicit(void *src, uint size);

void *create_and_copy(void *src, uint (*get_size)(void *data));

uint string_data_size(string_t data);

void string_free(string_t *data);

#endif  // UTILS_H
