//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef REQUEST_H
#define REQUEST_H

#include "../../utils/constants.h"
#include "document.h"

typedef struct {
	request_type_t type;
	document_t *document;
} request_t;

// ==================== Constructor(s) ====================

request_t *request_init(request_type_t type, document_t* document);

// ==================== Memory ====================

void request_free(request_t **request);

// ==================== Functional ====================

bool request_equal(request_t *request1, request_t *request2);

uint request_size(__attribute__((unused)) request_t *request);

#endif // REQUEST_H
