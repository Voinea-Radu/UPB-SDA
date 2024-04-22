//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hash_map.h"

hash_map_t *hash_map_init(uint capacity, uint (*hash)(void *key), bool (*compare_keys)(void *key1, void *key2))
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
	map->hash = hash;
	map->compare_keys = compare_keys;

	return map;
}

bool hash_map_put(hash_map_t *map, void *key, void *value)
{
#if DEBUG
	printf("hash_map_put\n");
	printf("hashing key: %s\n", (string_t)key);
	printf("hash %u\n", map->hash(key));
	printf("capacity %u\n", map->capacity);
	printf("index %u\n", map->hash(key) % map->capacity);
#endif

	uint index = map->hash(key) % map->capacity;
	hash_map_entry_t *entry = map->entries[index];

	while (entry) {
		if (map->compare_keys(entry->key, key)) {
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
	map->size++;

	return true;
}

void *hash_map_get(hash_map_t *map, void *key)
{
#if DEBUG
	printf("hash_map_get\n");
	printf("hashing key: %s\n", (string_t)key);
	printf("hash %u\n", map->hash(key));
	printf("capacity %u\n", map->capacity);
	printf("index %u\n", map->hash(key) % map->capacity);
#endif
	uint index = map->hash(key) % map->capacity;
	hash_map_entry_t *entry = map->entries[index];

	while (entry) {
		if (map->compare_keys(entry->key, key))
			return entry->value;

		entry = entry->next;
	}

	return NULL;
}

void hash_map_remove(hash_map_t *map, void *key)
{
	uint index = map->hash(key) % map->capacity;
	hash_map_entry_t *entry = map->entries[index];
	hash_map_entry_t *prev = NULL;

	while (entry) {
		if (map->compare_keys(entry->key, key)) {
			if (prev)
				prev->next = entry->next;
			else
				map->entries[index] = entry->next;

			free(entry);
			--map->size;
			return;
		}

		prev = entry;
		entry = entry->next;
	}

}

void hash_map_free(hash_map_t **map)
{
	for (uint i = 0; i < (*map)->capacity; ++i) {
		hash_map_entry_t *entry = (*map)->entries[i];

		while (entry) {
			hash_map_entry_t *next = entry->next;
			free(entry);
			entry = next;
		}
	}

	free((*map)->entries);
	free(*map);
	*map = NULL;
}

void hash_map_print(hash_map_t *map, string_t prefix, void (*print_entry)(string_t, void *, void *))
{
	for (uint i = 0; i < map->capacity; ++i) {
		hash_map_entry_t *entry = map->entries[i];

		while (entry) {
			print_entry(prefix, entry->key, entry->value);
			entry = entry->next;
		}
	}
}
