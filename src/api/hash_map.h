/*
 * Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
 */

#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdbool.h>
#include "constants.h"

typedef struct hash_map_entry {
	void *key;
	void *value;
	struct hash_map_entry *next;
} hash_map_entry_t;

typedef struct {
	hash_map_entry_t **entries;
	unsigned int capacity;
	unsigned int size;

	uint (*hash)(void *key);
} hash_map_t;

hash_map_t *hash_map_init(unsigned int capacity, uint (*hash)(void *key));

bool hash_map_put(hash_map_t *map, void *key, void *value);

void *hash_map_get(hash_map_t *map, void *key);

void hash_map_remove(hash_map_t *map, void *key);

void hash_map_free(hash_map_t **map);

void hash_map_print(hash_map_t *map, string_t prefix);

#endif //HASH_MAP_H
