//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include "hash_map.h"
#include "../utils/utils.h"

hash_map_t *hash_map_init(uint32_t size, uint32_t (*hash_function)(void *key),
						  bool (*compare_keys)(void *key1, void *key2))
{
	hash_map_t *map = safe_malloc(sizeof(hash_map_t));

	map->size = size;
	map->nodes = safe_calloc(size * sizeof(hash_node_t *));
	map->hash_function = hash_function;
	map->compare_keys = compare_keys;

	return map;
}

void hash_map_put(hash_map_t *map, void *key, void *value)
{
	uint32_t index = map->hash_function(key) % map->size;
	hash_node_t *node = safe_malloc(sizeof(hash_node_t));
	node->key = key;
	node->value = value;
	node->next = map->nodes[index];
	map->nodes[index] = node;
}

void *hash_map_get(hash_map_t *map, void *key)
{
	uint index = map->hash_function(key) % map->size;
	hash_node_t *node = map->nodes[index];

	while (node) {
		if (map->compare_keys(node->key, key))
			return node->value;
		node = node->next;
	}

	return NULL;
}

void hash_map_delete(hash_map_t *map, void *key)
{
	uint index = map->hash_function(key) % map->size;
	hash_node_t *node = map->nodes[index];
	hash_node_t *prev = NULL;
	while (node && node->key != key) {
		prev = node;
		node = node->next;
	}

	if (!node)
		return;

	if (!prev)
		map->nodes[index] = node->next;
	else
		prev->next = node->next;

	free(node);
}

void hash_map_free(hash_map_t *map)
{
	for (uint32_t i = 0; i < map->size; i++) {
		hash_node_t *node = map->nodes[i];
		while (node) {
			hash_node_t *next = node->next;
			free(node);
			node = next;
		}
	}
	free(map->nodes);
	free(map);
}

