/*
 * Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
 */

#include <stdlib.h>
#include "api/hash_map.h"

hash_map_t *hash_map_init(unsigned int capacity)
{
	hash_map_t *map = malloc(sizeof(hash_map_t));

	if (!map)
		return NULL;

	map->entries = calloc(capacity, sizeof(hash_map_entry_t *));

	if (!map->entries) {
		free(map);
		return NULL;
	}

	map->capacity = capacity;
	map->size = 0;

	return map;
}

bool hash_map_put(hash_map_t *map, void *key, void *value)
{
	unsigned int index = map->hash(key) % map->capacity;
	hash_map_entry_t *entry = map->entries[index];

	while (entry) {
		if (entry->key == key) {
			entry->value = value;
			return true;
		}

		entry = entry->next;
	}

	entry = malloc(sizeof(hash_map_entry_t));

	if (!entry)
		return false;

	entry->key = key;
	entry->value = value;
	entry->next = map->entries[index];
	map->entries[index] = entry;
	++map->size;

	return true;
}

void *hash_map_get(hash_map_t *map, void *key)
{
	unsigned int index = map->hash(key) % map->capacity;
	hash_map_entry_t *entry = map->entries[index];

	while (entry) {
		if (entry->key == key)
			return entry->value;

		entry = entry->next;
	}

	return NULL;
}