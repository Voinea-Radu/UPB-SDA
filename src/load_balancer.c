/*
 * Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
 */

#include "api/load_balancer.h"
#include "api/server.h"

load_balancer_t *load_balancer_init(bool enable_vnodes)
{
	/* TODO */
	return NULL;
}

void load_balancer_add_server(load_balancer_t *load_balancer, int server_id, int cache_size)
{
	/* TODO: Remove test_server after checking the server_t implementation */
	// load_balancer->test_server = server_init(cache_size);
}

void load_balancer_remove_server(load_balancer_t *load_balancer, int server_id)
{
	/* TODO */
}

response_t *load_balancer_forward_request(load_balancer_t *load_balancer, request_t *request)
{
	/* TODO */
	return NULL;
}

void load_balancer_free(load_balancer_t **load_balancer)
{
	/* TODO: get rid of test_server after testing the server_t implementation */
	// server_free(&(*load_balancer)->test_server);
	free(*load_balancer);

	*load_balancer = NULL;
}


