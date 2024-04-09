/*
 * Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
 */

#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <stdbool.h>
#include <sys/types.h>
#include "constants.h"

typedef struct cache {
	// TODO
} cache_t;

cache_t *cache_init(uint cache_capacity);

bool cache_is_full(cache_t *cache);

void cache_free(cache_t **cache);

/**
 * cache_put() - Adds a new pair in our cache.
 * 
 * @param cache: Cache where the key-value pair will be stored.
 * @param key: Key of the pair.
 * @param value: Value of the pair.
 * @param evicted_key: The function will RETURN via this parameter the
 *      key removed from cache if the cache was full.
 * 
 * @return - true if the key was added to the cache,
 *      false if the key already existed.
 */
bool cache_put(cache_t *cache, void *key, void *value, void **evicted_key);

/**
 * cache_get() - Retrieves the value associated with a key.
 * 
 * @param cache: Cache where the key-value pair is stored.
 * @param key: Key of the pair.
 * 
 * @return - The value associated with the key,
 *      or NULL if the key is not found.
 */
void *cache_get(cache_t *cache, void *key);

/**
 * cache_remove() - Removes a key-value pair from the cache.
 * 
 * @param cache: Cache where the key-value pair is stored.
 * @param key: Key of the pair.
*/
void cache_remove(cache_t *cache, void *key);

void cache_log_miss(string_t key);

void cache_log_hit(string_t key);

void cache_log_miss_with_eviction(string_t key, string_t evicted_key);

#endif // LRU_CACHE_H
