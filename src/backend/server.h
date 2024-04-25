//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef SERVER_H
#define SERVER_H

#include "cache.h"
#include "database.h"
#include "dto/document.h"
#include "dto/request.h"
#include "dto/response.h"

typedef struct {
	uint server_id;
	uint hash;

	queue_t *task_queue;
	database_t *database;
	cache_t *cache;
} server_t;

// ==================== Constructor(s) ====================

server_t *server_init(uint cache_size, uint server_id);

// ==================== Memory ====================

/**
 * @brief Should deallocate completely the memory used by server_t,
 *     taking care of deallocating the elements in the queue, if any,
 *     without executing the tasks
 */
void server_free(server_t **server);

// ==================== Functional ====================

/**
 * server_handle_request() - Receives a request_t from the load balancer
 *      and processes it according to the request_t type
 * 
 * @param server: Server which processes the request_t.
 * @param request: Request to be processed.
 * 
 * @return response_t*: Response of the requested operation, which will
 *      then be printed in main.
 * 
 * @brief Based on the type of request_t, should call the appropriate
 *     solver, and should execute the tasks from queue if needed (in
 *     this case, after executing each task, PRINT_RESPONSE should
 *     be called).
 */
response_t *server_handle_request(server_t *server, request_t *request, bool execute_immediately, bool bypass_cache);

void execute_task_queue(server_t *server);

uint server_size(server_t *server);

bool server_compare(server_t *server1, server_t *server2);

document_t **server_get_all_documents(server_t *server, uint *size);

void remove_document(server_t *server, document_t *document);

#endif // SERVER_H
