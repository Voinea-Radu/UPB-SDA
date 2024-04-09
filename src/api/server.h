/*
 * Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
 */

#ifndef SERVER_H
#define SERVER_H

#include "utils.h"
#include "constants.h"
#include "cache.h"

#define TASK_QUEUE_SIZE         1000
#define MAX_LOG_LENGTH          100
#define MAX_RESPONSE_LENGTH     4096

typedef struct {
	cache_t *cache;

	// TODO: add needed fields
} server_t;

typedef struct {
	string_t name;
	string_t content;
} document_t;

typedef struct {
	request_type_t type;
	document_t document;
} request_t;

typedef struct {
	string_t server_log;
	string_t server_response;
	int server_id;
} response_t;


server_t *server_init(uint cache_size);

/**
 * @brief Should deallocate completely the memory used by server_t,
 *     taking care of deallocating the elements in the queue, if any,
 *     without executing the tasks
 */
void server_free(server_t **server);

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
response_t *server_handle_request(server_t *server, request_t *request);

void response_print(response_t *response);

#endif  // SERVER_H
