//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdbool.h>
#include "../utils/constants.h"
#include "../backend/dto/document.h"

/**
 * @brief The entry of a hash map
 * @field key 		The key of the entry
 * @field value 	The value of the entry
 * @field next 	The next entry in the list
 */
typedef struct hash_map_entry {
	void *key;
	void *value;
	struct hash_map_entry *next;
} hash_map_entry_t;

/**
 * @brief The hash map structure
 * @field entries 			The entries of the hash map
 * @field capacity 			The capacity of the hash map
 * @field size 				The size of the hash map
 * @field hash_key 			The function that hashes the key
 * @field key_compare 		The function that compares two keys
 * @field key_get_size 		The function that gets the size of a key
 * @field key_free 			The function that frees a key
 * @field value_get_size 	The function that gets the size of a value
 * @field value_free 		The function that frees a value
 */
typedef struct {
	hash_map_entry_t **entries;
	uint capacity;
	uint size;

	// Key functions
	uint (*hash_key)(void *key);

	bool (*key_compare)(void *key1, void *key2);

	uint (*key_get_size)(void *key);

	void (*key_free)(void **key);

	// Value functions
	uint (*value_get_size)(void *value);

	void (*value_free)(void **key);
} hash_map_t;

// ==================== Constructor(s) ====================

/**
 * @brief Initializes a new hash map
 * @param capacity 			The capacity of the hash map
 * @param key_hash 			The function that hashes a key
 * @param key_compare 		The function that compares two keys
 * @param key_get_size 		The function that gets the size of a key
 * @param value_get_size 	The function that gets the size of a value
 * @param key_free 			The function that frees a key
 * @param value_free 		The function that frees a value
 * @return A pointer to the newly created hash map
 */
hash_map_t *hash_map_init(uint capacity, uint (*key_hash)(void *key),
						  bool (*key_compare)(void *key1, void *key2),
						  uint (*key_get_size)(void *key),
						  uint (*value_get_size)(void *value),
						  void (*key_free)(void **key),
						  void (*value_free)(void **key));

// ==================== Memory ====================

/**
 * @brief Deallocates the memory used by a hash map
 * @param map 	The hash map to deallocate
 */
void hash_map_free(hash_map_t **map);

// ==================== Functional ====================

/**
 * @brief Puts a new key-value pair in the hash map
 * @param map 		The hash map to put the key-value pair in
 * @param key 		The key of the pair
 * @param value 	The value of the pair
 */
void hash_map_put(hash_map_t *map, void *key, void *value);

/**
 * @brief Gets the value associated with a key from the hash map
 * @param map 	The hash map to get the value from
 * @param key 	The key of the pair
 * @return The value associated with the key
 */
void *hash_map_get(hash_map_t *map, void *key);

/**
 * @brief Removes the key-value pair associated with the key from the hash map
 * @param map 	The hash map to remove the key-value pair from
 * @param key 	The key of the pair
 */
void *hash_map_remove(hash_map_t *map, void *key);

/**
 * @brief Checks if the hash map contains a key
 * @param map 	The hash map to check
 * @param key 	The key to check
 * @return True if the hash map contains the key, false otherwise
 */
void hash_map_print(hash_map_t *map, string_t prefix,
					void (*print_entry)(string_t, void *, void *));

/**
 * @brief Gets the keys of the hash map
 * @param map 	The hash map to get the keys from
 * @return The keys of the hash map
 */
document_t **hash_map_get_values(hash_map_t *map);

#endif  // HASH_MAP_H
