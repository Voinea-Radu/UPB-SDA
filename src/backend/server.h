//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef SERVER_H
#define SERVER_H

#include "cache.h"
#include "database.h"
#include "document.h"


typedef struct {
	uint server_id;

	queue_t *task_queue;
	database_t *database;
	cache_t *cache;
} server_t;

typedef struct {
	request_type_t type;
	document_t document;
} request_t;

typedef struct {
	uint server_id;

	string_t server_log;
	string_t server_response;
} response_t;

// ================== Init Functions ==================

server_t *server_init(uint cache_size, uint server_id);

request_t *request_init(request_type_t type, document_t document);

response_t *response_init(uint server_id, string_t server_log, string_t server_response);

// ================== Memory Functions ==================

/**
 * @brief Should deallocate completely the memory used by server_t,
 *     taking care of deallocating the elements in the queue, if any,
 *     without executing the tasks
 */
void server_free(server_t **server);

void response_free(response_t **response);

void request_free(request_t **request);

// ================== Functional Functions ==================

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
response_t *server_handle_request(server_t *server, request_t *request, bool execute_immediately);

void response_print(response_t *response);

void execute_task_queue(server_t *server);

uint document_hash(document_t *document);

bool request_equal(request_t *request1, request_t *request2);

uint request_size(request_t *request);

#if DEBUG
string_t queued_task_to_string(request_t *request);

void server_print(server_t *server, string_t prefix);
#endif // DEBUG

#endif // SERVER_H
