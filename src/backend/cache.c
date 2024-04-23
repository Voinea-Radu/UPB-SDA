//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include <string.h>

#include "cache.h"
#include "../utils/utils.h"

cache_t *cache_init(uint cache_capacity)
{
	cache_t *cache = safe_malloc(sizeof(cache_t));

	cache->map = hash_map_init(cache_capacity, (uint (*)(void *))hash_string, (bool (*)(void *, void *))string_equals,
							   (uint (*)(void *))string_data_size, (uint (*)(void *))string_data_size,
							   (void (*)(void **))string_free, (void (*)(void **))string_free);
	cache->queue = queue_init((bool (*)(void *, void *))string_equals, (uint (*)(void *))string_data_size, (void (*)(void **))string_free);
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

document_t *cache_put(cache_t *cache, document_t* document)
{
	document_t *evicted_document = NULL;

	if (cache_is_full(cache)) {
		string_t evicted_key = queue_dequeue(cache->queue);
		string_t evicted_content = hash_map_remove(cache->map, evicted_key);

		evicted_document = document_init(evicted_key, evicted_content);
	}

	queue_enqueue(cache->queue, document->name);
	hash_map_put(cache->map, document->name, document->content);

	return evicted_document;
}

string_t cache_get(cache_t *cache, string_t key)
{
	string_t output = hash_map_get(cache->map, key);

	if(queue_remove(cache->queue, key)){
		queue_enqueue(cache->queue, key);
	}

	return output;
}

#if DEBUG

string_t key_to_string(void *key)
{
	return (string_t)key;
}

void cache_print_entry(string_t prefix, string_t key, string_t value)
{
	debug_log("%s- Key: %s, Value: %s\n", prefix, key, value);
}

void cache_print(cache_t *cache, string_t prefix)
{
	string_t new_prefix = increase_prefix(prefix);

	debug_log("%sCache capacity: %d\n", prefix, cache->capacity);
	debug_log("%sCache size: %d\n", prefix, cache->map->size);

	debug_log("%sCache entries:\n", prefix);

	hash_map_print(cache->map, new_prefix, (void (*)(string_t, void *, void *))cache_print_entry);

	debug_log("%sCache history:\n", prefix);
	queue_print(cache->queue, key_to_string, new_prefix, false);

	free(new_prefix);
}

#endif
