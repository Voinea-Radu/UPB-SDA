//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef DEBUG_H
#define DEBUG_H

#include "utils.h"
#include "../backend/cache.h"
#include "../backend/dto/request.h"
#include "../backend/server.h"
#include "../backend/load_balancer.h"

#if DEBUG

/**
 * @brief Prints the cache
 * @param cache  		The cache to print
 * @param prefix  		The prefix to print before the cache. This is used when
 * 						printing nested structures
 */
void cache_print(cache_t *cache, string_t prefix);

string_t queued_task_to_string(request_t *request);

void server_print(server_t *server, string_t prefix);

void database_print(database_t *database, string_t prefix);

void queue_print(queue_t *queue, string_t (*to_string_function)(void*),string_t prefix, bool should_free);

void load_balancer_print(load_balancer_t *load_balancer);

#endif // DEBUG

#endif // DEBUG_H
