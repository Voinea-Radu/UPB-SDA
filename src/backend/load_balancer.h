//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef LOAD_BALANCER_H
#define LOAD_BALANCER_H

#include "server.h"
#include "../generic/linked_list.h"

#define MAX_SERVERS             99999

/**
 * @brief The load balancer structure
 * @field hash_document 	The function that hashes a document
 * @field servers 			The list of servers
 * @field servers_count 	The number of servers
 * @field enable_vnodes 	Whether to enable virtual nodes
 */
typedef struct load_balancer {
	uint (*hash_document)(document_t *document);

	linked_list_t *servers;
	uint servers_count;

	bool enable_vnodes;
} load_balancer_t;


/**
 * @brief Initializes a load balancer
 * @param enable_vnodes 	Whether to enable virtual nodes
 * @return A pointer to the newly created load balancer
 */
load_balancer_t *load_balancer_init(bool enable_vnodes);

/**
 * @brief Deallocates the memory used by a load balancer
 * @param load_balancer 	The load balancer to deallocate
 */
void load_balancer_free(load_balancer_t **load_balancer);

/**
 * @brief Adds a new server to the load balancer
 * @param load_balancer The load balancer to add the server to
 * @param server_id 	The id of the server
 * @param cache_size 	The size of the cache of the server
 */
void load_balancer_add_server(load_balancer_t *load_balancer, int server_id,
							  int cache_size);

/**
 * @brief Adds a new server to the load balancer
 * @param load_balancer The load balancer to add the server to
 * @param server 		The server to add
 * @return The server that was added
 */
server_t *__load_balancer_add_server(load_balancer_t *load_balancer,
									 server_t *server);

/**
 * @brief Removes a server from the load balancer
 * @param load_balancer The load balancer to remove the server from
 * @param server_id 	The id of the server to remove
 */
void load_balancer_remove_server(load_balancer_t *load_balancer,
								 uint server_id);

/**
 * @brief Gets the target server for a document
 * @param load_balancer The load balancer to get the target server from
 * @param document 		The document to get the target server for
 * @return The target server
 */
server_t *load_balancer_get_target_server(load_balancer_t *load_balancer,
										  document_t *document);

/**
 * @brief Gets the server with the given hash
 * @param load_balancer 	The load balancer to get the server from
 * @param hash 			The hash of the server
 * @return The server with the given hash
 */
server_t *load_balancer_get_server(load_balancer_t *load_balancer,
								   uint hash);

/**
 * @brief Forwards a request to the target server
 * @param load_balancer 		The load balancer to forward the request with
 * @param request 				The request to forward
 * @param execute_immediately 	Whether to execute the request immediately
 * @param bypass_cache 			Whether to bypass the cache
 * @return The response of the server
 */
response_t *load_balancer_forward_request(load_balancer_t *load_balancer,
										  request_t *request,
										  bool execute_immediately,
										  bool bypass_cache);

/**
 * @brief Gets the replica id of a server
 * @param server_id 		The id of the server
 * @param replica_number 	The number of the replica
 * @return The id of the replica
 */
uint get_replica_id(uint server_id, int replica_number);

#endif  // LOAD_BALANCER_H
