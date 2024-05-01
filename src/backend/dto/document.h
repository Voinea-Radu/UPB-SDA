//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "../../utils/constants.h"

/**
 * @brief A document structure
 *
 * @field name 	The name of the document
 * @field content 	The content of the document
 */
typedef struct {
	string_t name;
	string_t content;
} document_t;
// ==================== Constructor(s) ====================

/**
 * @brief Initializes a document
 * @param name 		The name of the document
 * @param content	The content of the document
 * @return A pointer to the newly created document
 */
document_t *document_init(string_t name, string_t content);

// ==================== Memory ====================

/**
 * @brief Deallocates the memory used by a document
 * @param document 	The document to deallocate
 */
void document_free(document_t **document);

// ==================== Functional ====================

/**
 * @brief Computes the hash of a document, by
 * @param document 	The document to hash
 * @return The hash of the document
 */
uint document_hash(document_t *document);

#endif  // DOCUMENT_H
