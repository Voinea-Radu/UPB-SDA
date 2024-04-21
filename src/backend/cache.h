//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <stdbool.h>
#include <sys/types.h>
#include "../utils/constants.h"
#include "../generic/hash_map.h"
#include "../generic/queue.h"

typedef struct cache {
	hash_map_t *map;
	queue_t *queue;

	uint capacity;
} cache_t;

cache_t *cache_init(uint cache_capacity);

bool cache_is_full(cache_t *cache);

void cache_free(cache_t **cache);

bool cache_put(cache_t *cache, string_t key, string_t value, string_t* evicted_key);

string_t cache_get(cache_t *cache, string_t key);

void cache_print(cache_t *cache, string_t prefix);

#endif // LRU_CACHE_H
