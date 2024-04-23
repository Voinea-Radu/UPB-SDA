//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include <string.h>
#include "hash_map.h"
#include "../utils/utils.h"

hash_map_t *hash_map_init(uint capacity, uint (*key_hash)(void *key), bool (*key_compare)(void *key1, void *key2),
						  uint (*key_get_size)(void *key), uint (*value_get_size)(void *value), void (*key_free)(void **key),
						  void (*value_free)(void **key))
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

	map->hash_key = key_hash;
	map->key_compare = key_compare;
	map->key_get_size = key_get_size;
	map->key_free = key_free;

	map->value_get_size = value_get_size;
	map->value_free = value_free;

	return map;
}

bool hash_map_put(hash_map_t *map, void *key, void *value)
{
	uint index = map->hash_key(key) % map->capacity;
	hash_map_entry_t *entry = map->entries[index];

	while (entry) {
		if (map->key_compare(entry->key, key)) {
			entry->value = create_and_copy(entry->value, map->value_get_size);
			return true;
		}

		entry = entry->next;
	}

	entry = malloc(sizeof(hash_map_entry_t));

	if (!entry)
		return false;

	entry->key = create_and_copy(key, map->key_get_size);
	entry->value = create_and_copy(value, map->value_get_size);
	entry->next = map->entries[index];
	map->entries[index] = entry;
	map->size++;

	return true;
}

void *hash_map_get(hash_map_t *map, void *key)
{
	uint index = map->hash_key(key) % map->capacity;
	hash_map_entry_t *entry = map->entries[index];

	while (entry) {
		if (map->key_compare(entry->key, key))
			return create_and_copy(entry->value, map->value_get_size);

		entry = entry->next;
	}

	return NULL;
}

void *hash_map_remove(hash_map_t *map, void *key)
{
	uint index = map->hash_key(key) % map->capacity;
	hash_map_entry_t *entry = map->entries[index];
	hash_map_entry_t *prev = NULL;

	while (entry) {
		if (map->key_compare(entry->key, key)) {
			if (prev)
				prev->next = entry->next;
			else
				map->entries[index] = entry->next;

			void *output = entry->value;

			map->key_free(entry->key);
			free(entry);

			--map->size;
			return output;
		}

		prev = entry;
		entry = entry->next;
	}

	return NULL;
}

void hash_map_free(hash_map_t **map)
{
	for (uint i = 0; i < (*map)->capacity; ++i) {
		hash_map_entry_t *entry = (*map)->entries[i];

		while (entry) {
			hash_map_entry_t *next = entry->next;

			(*map)->key_free(&entry->value);
			(*map)->value_free(&entry->value);
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
