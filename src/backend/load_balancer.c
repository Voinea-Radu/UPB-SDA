//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include "load_balancer.h"
#include "server.h"
#include "../utils/utils.h"

load_balancer_t *load_balancer_init(bool enable_vnodes)
{
	load_balancer_t *load_balancer = safe_malloc(sizeof(load_balancer_t));

	load_balancer->enable_vnodes = enable_vnodes;
	load_balancer->hash_document = document_hash;
	load_balancer->servers_count = 0;

	return load_balancer;
}

void load_balancer_add_server(load_balancer_t *load_balancer, int server_id, int cache_size)
{
	load_balancer->servers_count++;
	uint index = uint_hash(server_id) % load_balancer->servers_count;

	load_balancer->servers[index] = server_init(cache_size, server_id);
}

void load_balancer_remove_server(load_balancer_t *load_balancer, int server_id)
{
	server_free(&load_balancer->servers[server_id]);
}

response_t *load_balancer_forward_request(load_balancer_t *load_balancer, request_t *request)
{
	uint index = load_balancer->hash_document(request->document) % load_balancer->servers_count;
	return server_handle_request(load_balancer->servers[index], request, false);
}

void load_balancer_free(load_balancer_t **load_balancer)
{
	for (uint i = 0; i < (*load_balancer)->servers_count; i++) {
		if ((*load_balancer)->servers[i]) {
			server_free(&(*load_balancer)->servers[i]);
		}
	}

	free(*load_balancer);
	*load_balancer = NULL;
}

