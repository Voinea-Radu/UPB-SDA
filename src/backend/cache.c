//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include <string.h>

#include "cache.h"
#include "../utils/utils.h"

cache_t *cache_init(uint cache_capacity)
{
	cache_t *cache = safe_malloc(sizeof(cache_t));

	cache->data = hash_map_init(cache_capacity,
								(uint (*)(void *))string_hash,
								(bool (*)(void *, void *))string_equals,
								(uint (*)(void *))string_data_size,
								(uint (*)(void *))string_data_size,
								(void (*)(void **))string_free,
								(void (*)(void **))string_free);
	cache->history = queue_init((bool (*)(void *, void *))string_equals,
								(uint (*)(void *))string_data_size,
								(void (*)(void **))string_free);

	return cache;
}

bool cache_is_full(cache_t *cache)
{
	return cache->data->size == cache->data->capacity;
}

void cache_free(cache_t **cache)
{
	hash_map_free(&(*cache)->data);
	queue_free(&(*cache)->history);

	free(*cache);
	*cache = NULL;
}

document_t *cache_put(cache_t *cache, document_t *document)
{
	return cache_put_explicit(cache, document->name, document->content);
}

document_t *cache_put_explicit(cache_t *cache, string_t key, string_t value)
{
	string_t lookup_value = hash_map_get(cache->data, key);

	if (lookup_value != NULL) {
		string_free(&lookup_value);

		queue_remove(cache->history, key);
		queue_enqueue(cache->history, key);
		hash_map_put(cache->data, key, value);
		return NULL;
	}
	document_t *evicted_document = NULL;

	if (cache_is_full(cache)) {
		string_t evicted_key = queue_dequeue(cache->history);
		string_t evicted_content = hash_map_remove(cache->data, evicted_key);

		evicted_document = document_init(evicted_key, evicted_content);

		string_free(&evicted_key);
		string_free(&evicted_content);
	}

	queue_enqueue(cache->history, key);
	hash_map_put(cache->data, key, value);

	return evicted_document;
}

string_t cache_get(cache_t *cache, string_t key)
{
	string_t output = hash_map_get(cache->data, key);

	if (output == NULL) {
		return NULL;
	}

	queue_remove(cache->history, key);
	queue_enqueue(cache->history, key);

	return output;
}


