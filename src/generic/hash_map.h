//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdbool.h>
#include "../utils/constants.h"

typedef struct hash_map_entry {
	void *key;
	void *value;
	struct hash_map_entry *next;
} hash_map_entry_t;

typedef struct {
	hash_map_entry_t **entries;
	uint capacity;
	uint size;

	uint (*hash)(void *key);
	bool (*compare_keys)(void *key1, void *key2);
} hash_map_t;

hash_map_t *hash_map_init(uint capacity, uint (*hash)(void *key), bool (*compare_keys)(void *key1, void *key2));

bool hash_map_put(hash_map_t *map, void *key, void *value);

void *hash_map_get(hash_map_t *map, void *key);

void hash_map_remove(hash_map_t *map, void *key);

void hash_map_free(hash_map_t **map);

void hash_map_print(hash_map_t *map, string_t prefix, void (*print_entry)(string_t, void *, void *));

#endif //HASH_MAP_H
