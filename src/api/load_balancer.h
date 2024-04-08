/*
 * Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
 */

#ifndef LOAD_BALANCER_H
#define LOAD_BALANCER_H

#include "server.h"

#define MAX_SERVERS             99999

typedef struct load_balancer {
	unsigned int (*hash_server)(server_t* server);

	unsigned int (*hash_document)(void *); // TODO make more specific

	// TODO: remove test_server after testing the functionality for a single server_t
	server_t *test_server;

	// TODO: add fields needed for a hashing with multiple server_t
} load_balancer_t;


load_balancer_t *load_balancer_init(bool enable_vnodes);

void load_balancer_free(load_balancer_t **load_balancer);

/**
 * load_balancer_add_server() - Adds a new server_t to the system.
 * 
 * @param load_balancer: Load balancer which distributes the work.
 * @param server_id: ID of the new server_t.
 * @param cache_size: Capacity of the new server_t's cache.
 * 
 * @brief The load balancer will generate 1 or 3 replica labels and will place
 * them inside the hash ring. The neighbor servers will distribute SOME of the
 * documents to the added server_t. Before distributing the documents, these
 * servers should execute all the tasks in their queues.
 */
void load_balancer_add_server(load_balancer_t *load_balancer, int server_id, int cache_size);

/**
 * load_balancer_remove_server() Removes a server_t from the system.
 * 
 * @param load_balancer: Load balancer which distributes the work.
 * @param server_id: ID of the server_t to be removed.
 * 
 * @brief The load balancer will remove the server_t (and its replicas) from
 * the hash ring and will distribute ALL documents stored on the removed
 * server_t to the "neighboring" servers.
 * 
 * Additionally, all the tasks stored in the removed server_t's queue
 * should be executed before moving the documents.
 */
void load_balancer_remove_server(load_balancer_t *load_balancer, int server_id);

/**
 * load_balancer_forward_request() - Forwards a request_t to the appropriate server_t.
 * 
 * @param load_balancer: Load balancer which distributes the work.
 * @param request: Request to be forwarded (relevant fields from the request_t are
 *        dynamically allocated, but the caller have to free them).
 * 
 * @return response_t* - Contains the response_t received from the server_t
 * 
 * @brief The load balancer will find the server_t which should handle the
 * request_t and will send the request_t to that server_t. The request_t will contain
 * the document name and/or content, which are dynamically allocated in load_balancer
 * and should be freed either here, either in server_handle_request, after
 * using them.
 */
response_t *load_balancer_forward_request(load_balancer_t *load_balancer, request_t *request);


#endif // LOAD_BALANCER_H
