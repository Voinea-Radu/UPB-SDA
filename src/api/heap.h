/*
Nume: Voinea Radu-Mihai
Grupa: 315 CA
*/

#ifndef TEMA1_HEAP_H
#define TEMA1_HEAP_H

#include <stdbool.h>
#include "linked_list.h"

typedef struct {
	uint64_t start_address;
	uint64_t size;
} heap_block_t;

typedef struct {
	uint64_t start_address;
	uint64_t block_size;
	linked_list_t *blocks;
} heap_pool_t;

typedef struct {
	uint64_t start_address;
	linked_list_t *pools;
	linked_list_t *used_blocks;
} heap_t;

heap_t create_heap(uint64_t start_address, uint64_t number_of_pools, uint64_t pool_size, bool reconstruction_type);

uint64_t heap_malloc(heap_t *heap, uint64_t size);

void dump_heap(heap_t *heap);

heap_t *new_heap(uint64_t start_address);

heap_pool_t *new_heap_pool(uint64_t start_address, uint64_t block_size);

heap_block_t *new_heap_block(uint64_t start_address, uint64_t size);

#endif //TEMA1_HEAP_H
