//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef REQUEST_H
#define REQUEST_H

#include <stdbool.h>
#include "../../utils/constants.h"
#include "document.h"

/**
 * @brief The type of a request
 * @field type 			The type of the request
 * @field document 		The document associated with the request
 * @field server_id 	The id of the server that the request is sent to
 */
typedef struct {
	request_type_t type;
	document_t *document;
	uint server_id;
} request_t;

// ==================== Constructor(s) ====================

/**
 * @brief Initializes a request
 * @param type  			The type of the request
 * @param document 			The document associated with the request
 * @param server_id 		The id of the server that the request is sent to
 * @return A pointer to the newly created request
 */
request_t *request_init(request_type_t type, document_t *document,
						uint server_id);

// ==================== Memory ====================

/**
 * @brief Deallocates the memory used by a request
 * @param request 	The request to deallocate
 */
void request_free(request_t **request);

// ==================== Functional ====================

/**
 * @brief Computes the hash of a request
 * @param request1 	The request to hash
 * @param request2  The request to hash
 * @return The hash of the request
 */
bool request_equal(request_t *request1, request_t *request2);

/**
 * @brief Computes the size of a request
 * @param request 	The request to hash
 * @return The size of the request
 */
uint request_size(__attribute__((unused)) request_t *request);

#endif  // REQUEST_H
