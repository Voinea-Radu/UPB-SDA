//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <sys/types.h>
#include <stdint-gcc.h>
#include <stdbool.h>

typedef struct hash_node_t {
	void *key;
	void *value;
	struct hash_node_t *next;
} hash_node_t;

typedef struct {
	uint32_t size;
	hash_node_t **nodes;

	uint32_t (*hash_function)(void *);
	bool (*compare_keys)(void *, void *);
} hash_map_t;

hash_map_t *hash_map_init(uint32_t size, uint32_t (*hash_function)(void *),
						  bool (*compare_keys)(void *, void *));

void hash_map_put(hash_map_t *map, void *key, void *value);

void *hash_map_get(hash_map_t *map, void *key);

void hash_map_delete(hash_map_t *map, void *key);

void hash_map_free(hash_map_t *map);

#endif // HASH_MAP_H
