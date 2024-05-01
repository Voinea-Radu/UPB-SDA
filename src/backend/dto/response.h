//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef RESPONSE_H
#define RESPONSE_H

#include <sys/types.h>
#include "../../utils/constants.h"

/**
 * @brief The type of a response
 * @field server_id 	The id of the server that the response is sent from
 * @field server_log 	The log of the server
 *
 */
typedef struct {
	uint server_id;

	string_t server_log;
	string_t server_response;
} response_t;

// ==================== Constructor(s) ====================

/**
 * @brief Initializes a response
 * @param server_id 		The id of the server that the response is sent from
 * @param server_log 		The log of the server
 * @param server_response 	The response of the server
 * @return A pointer to the newly created response
 */
response_t *response_init(string_t server_log, string_t server_response);

// ==================== Memory ====================

/**
 * @brief Deallocates the memory used by a response
 * @param response 	The response to deallocate
 */
void response_free(response_t **response);

// ==================== Functional ====================

/**
 * @brief Computes the hash of a response
 * @param response1 	The response to hash
 * @param response2  The response to hash
 * @return The hash of the response
 */
void response_print(response_t *response);

#endif  // RESPONSE_H
