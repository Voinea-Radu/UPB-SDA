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
#include "document.h"

/**
 * @brief The cache structure. This is a Least Recently Used (LRU) cache. See
 * <a href="https://en.wikipedia.org/wiki/Cache_replacement_policies
 * #Least_recently_used_(LRU)">LRU Cache</a> for more information.
 *
 * @param map  		The hash map that stores the key-value pairs. The key is a
 * 					string (the name of the document to be cached) and the value
 * 					is a string (the content of the document)
 * @param queue  	The queue that stores the keys in the order they were
 * 					accessed. The elements in the queue are strings (the name
 * 					of the document accessed)
 * @param capacity 	The maximum number of elements the cache can store
 */
typedef struct cache {
	hash_map_t *map; // HashMap<String, String>
	queue_t *queue; // Queue<String>

	uint capacity;
} cache_t;

// ==================== Constructor(s) ====================

/**
 * @brief Initializes a new cache
 *
 * @param cache_capacity  The maximum number of elements the cache can store
 * @return The newly created cache
 */
cache_t *cache_init(uint cache_capacity);

// ==================== Management ====================

/**
 * @brief Checks if the cache is full
 *
 * @param cache 	The cache to check
 * @return True if the cache is full, false otherwise
 */
bool cache_is_full(cache_t *cache);

/**
 * @brief Puts a new key-value pair in the cache. If the cache is full, it
 * evicts the least recently used element. See
 * <a href="https://en.wikipedia.org/wiki/Cache_replacement_policies
 * #Least_recently_used_(LRU)">LRU Cache</a> for more information.
 *
 * @param cache 	The cache to put the key-value pair in
 * @param document	The document to put in the cache
 * @return The document that was evicted from the cache
 */
document_t *cache_put(cache_t *cache, document_t *document);

/**
 * @brief Gets the value associated with the key from the cache
 *
 * @param cache 	The cache to get the value from
 * @param key 		The key to get the value for
 * @return The value associated with the key
 */
string_t cache_get(cache_t *cache, string_t key);

// ==================== Memory ====================

/**
 * @brief Frees the cache
 *
 * @param cache	The cache to free
 */
void cache_free(cache_t **cache);

#if DEBUG

/**
 * @brief Prints the cache
 * @param cache  		The cache to print
 * @param prefix  		The prefix to print before the cache. This is used when
 * 						printing nested structures
 */
void cache_print(cache_t *cache, string_t prefix);

#endif // DEBUG

#endif // LRU_CACHE_H
