//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef LOAD_BALANCER_H
#define LOAD_BALANCER_H

#include "server.h"
#include "../generic/linked_list.h"

#define MAX_SERVERS             99999

typedef struct load_balancer {
	uint (*hash_document)(document_t *document);

	linked_list_t *servers;
	uint servers_count;

	bool enable_vnodes;
} load_balancer_t;


load_balancer_t *load_balancer_init(bool enable_vnodes);

void load_balancer_free(load_balancer_t **load_balancer);

void load_balancer_add_server(load_balancer_t *load_balancer, int server_id,
							  int cache_size);

server_t *__load_balancer_add_server(load_balancer_t *load_balancer,
									 server_t *server);

void load_balancer_remove_server(load_balancer_t *load_balancer,
								 uint server_id);

server_t *load_balancer_get_target_server(load_balancer_t *load_balancer,
										  document_t *document);

server_t *load_balancer_get_server(load_balancer_t *load_balancer,
								   uint hash);


response_t *load_balancer_forward_request(load_balancer_t *load_balancer,
										  request_t *request,
										  bool execute_immediately,
										  bool bypass_cache);

uint get_replica_id(uint server_id, int replica_number);

#endif  // LOAD_BALANCER_H
