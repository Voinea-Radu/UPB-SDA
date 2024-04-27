//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef RESPONSE_H
#define RESPONSE_H

#include <sys/types.h>
#include "../../utils/constants.h"

typedef struct {
	uint server_id;

	string_t server_log;
	string_t server_response;
} response_t;

// ==================== Constructor(s) ====================

response_t *response_init(string_t server_log, string_t server_response);

// ==================== Memory ====================

void response_free(response_t **response);

// ==================== Functional ====================

void response_print(response_t *response);

#endif  // RESPONSE_H
