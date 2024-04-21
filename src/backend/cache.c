//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include <stdio.h>
#include <string.h>

#include "cache.h"
#include "../utils/utils.h"

cache_t *cache_init(uint cache_capacity)
{
	cache_t *cache = safe_malloc(sizeof(cache_t));

	cache->map = hash_map_init(cache_capacity, (uint (*)(void *))hash_string);
	cache->queue = queue_init();
	cache->capacity = cache_capacity;

	return cache;
}

bool cache_is_full(cache_t *cache)
{
	return cache->map->size == cache->capacity;
}

void cache_free(cache_t **cache)
{
	hash_map_free(&(*cache)->map);
	queue_free(&(*cache)->queue);

	free(*cache);
	*cache = NULL;
}

bool cache_put(cache_t *cache, string_t key, string_t value, string_t* evicted_key)
{
	if (!hash_map_put(cache->map, key, value)) {
		return false;
	}

	if (cache_is_full(cache)) {
		*evicted_key = queue_dequeue(cache->queue);
		hash_map_remove(cache->map, *evicted_key);
	}

	queue_enqueue(cache->queue, key);
	hash_map_put(cache->map, key, value);

	return true;
}

string_t cache_get(cache_t *cache, string_t key)
{
	void *value = hash_map_get(cache->map, key);

	if (value == NULL) {
		return NULL;
	}

	return value;
}

#if DEBUG

string_t key_to_string(void *key)
{
	return (string_t)key;
}

void cache_print(cache_t *cache, string_t prefix)
{
	string_t new_prefix = increase_prefix(prefix);

	printf("%sCache capacity: %d\n", prefix, cache->capacity);
	printf("%sCache size: %d\n", prefix, cache->map->size);

	printf("%sCache entries:\n", prefix);

	hash_map_print(cache->map, new_prefix);

	printf("%sCache history:\n", prefix);
	queue_print(cache->queue, key_to_string, new_prefix);
}

#endif
