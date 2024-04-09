/*
 * Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
 */

#include "api/load_balancer.h"
#include "api/server.h"

load_balancer_t *load_balancer_init(bool enable_vnodes)
{
	load_balancer_t *load_balancer = safe_malloc(sizeof(load_balancer_t));

	load_balancer->enable_vnodes = enable_vnodes;
	load_balancer->hash_document = hash_document;
	load_balancer->servers_count = 0;

	return load_balancer;
}

void load_balancer_add_server(load_balancer_t *load_balancer, int server_id, int cache_size)
{
	load_balancer->servers[server_id] = server_init(cache_size);
	load_balancer->servers_count++;
}

void load_balancer_remove_server(load_balancer_t *load_balancer, int server_id)
{
	server_free(&load_balancer->servers[server_id]);
}

response_t *load_balancer_forward_request(load_balancer_t *load_balancer, request_t *request)
{
	uint index = load_balancer->hash_document(&request->document) % load_balancer->servers_count;
	return server_handle_request(load_balancer->servers[index], request);
}

void load_balancer_free(load_balancer_t **load_balancer)
{
	for (int i = 0; i < (*load_balancer)->servers_count; i++) {
		if ((*load_balancer)->servers[i]) {
			server_free(&(*load_balancer)->servers[i]);
		}
	}

	free(*load_balancer);
	*load_balancer = NULL;
}

uint hash_document(document_t *document)
{
	return hash_string(document->name);
}


