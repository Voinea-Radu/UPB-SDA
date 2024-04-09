/*
 * Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
 */

#include <stdio.h>
#include <string.h>
#include "api/cache.h"
#include "api/utils.h"

cache_t *cache_init(unsigned int cache_capacity)
{
	/* TODO */
	return NULL;
}

bool cache_is_full(cache_t *cache)
{
	/* TODO */
	return false;
}

void cache_free(cache_t **cache)
{
	/* TODO */
}

bool cache_put(cache_t *cache, void *key, void *value,
			   void **evicted_key)
{
	/* TODO */
	return false;
}

void *cache_get(cache_t *cache, void *key)
{
	/* TODO */
	return NULL;
}

void cache_remove(cache_t *cache, void *key)
{
	/* TODO */
}

void cache_log_miss(string_t key)
{
#if DEBUG
	printf(LOG_CACHE_MISS, key);
#endif // DEBUG
}

void cache_log_hit(string_t key)
{
#if DEBUG
	printf(LOG_CACHE_HIT, key);
#endif // DEBUG
}

void cache_log_miss_with_eviction(string_t key, string_t evicted_key)
{
#if DEBUG
	printf(LOG_CACHE_EVICT, key, evicted_key);
#endif // DEBUG
}

