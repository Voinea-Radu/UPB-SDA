//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdbool.h>
#include "../utils/constants.h"
#include "../backend/dto/document.h"

typedef struct hash_map_entry {
	void *key;
	void *value;
	struct hash_map_entry *next;
} hash_map_entry_t;

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

hash_map_t *hash_map_init(uint capacity, uint (*key_hash)(void *key), bool (*key_compare)(void *key1, void *key2),
						  uint (*key_get_size)(void *key), uint (*value_get_size)(void *value), void (*key_free)(void **key),
						  void (*value_free)(void **key));

// ==================== Memory ====================

void hash_map_free(hash_map_t **map);

// ==================== Functional ====================

void hash_map_put(hash_map_t *map, void *key, void *value);

void *hash_map_get(hash_map_t *map, void *key);

void *hash_map_remove(hash_map_t *map, void *key);

void hash_map_print(hash_map_t *map, string_t prefix, void (*print_entry)(string_t, void *, void *));

document_t **hash_map_get_values(hash_map_t *map);

#endif //HASH_MAP_H
