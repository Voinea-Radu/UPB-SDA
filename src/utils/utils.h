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

/**
 * @brief Checks a condition and exits the program if the condition is false
 *
 * @param condition	The condition to check
 * @param message	The message to print if the condition is false
*/
void check_or_exit(bool condition, string_t message);

/**
 * @brief Exits the program with an error message
 *
 * @param message	The error message
*/
void exit_with_error(string_t message);

/**
 * @brief Compares two strings
 * @param string1  	The first string
 * @param string2	The second string
 * @return
 */
bool string_equals(string_t string1, string_t string2);

/**
 * @brief Returns the string representation of a request type
 * @param request_type  The request type
 * @return  The string representation of the request type
 */
string_t get_request_type_str(request_type_t request_type);

/**
 * @brief Returns the request type from a string
 * @param request_type_str  The string representation of the request type
 * @return  The request type
 */
request_type_t get_request_type(string_t request_type_str);

/**
 * @brief Returns the pointer to the newly allocated memory
 * @param size The size of the memory to allocate
 * @return The pointer to the newly allocated memory
 */
void *safe_malloc(size_t size);

/**
 * @brief Returns the pointer to the newly allocated memory
 * @param ptr The pointer to the memory to reallocate
 * @param size The size of the memory to allocate
 * @return The pointer to the newly allocated memory
 */
void *safe_realloc(void *ptr, size_t size);

/**
 * @brief Returns the pointer to the newly allocated memory
 * @param size The size of the memory to allocate
 * @return The pointer to the newly allocated memory
 */
void *safe_calloc(size_t size);

/**
 * @brief Converts a string to an integer
 * @param str The string to convert
 * @return The integer value of the string
 */
int str_to_int(string_t str);

/**
 * @brief Reads a quoted string from a buffer
 * @param buffer		The buffer to read from
 * @param buffer_len	The length of the buffer
 * @param start 		The start index of the string
 * @param end 			The end index of the string
 */
void read_quoted_string(string_t buffer, int buffer_len, int *start, int *end);


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
