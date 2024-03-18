/*
Nume: Voinea Radu-Mihai
Grupa: 315 CA
*/

#ifndef TEMA1_HEAP_H
#define TEMA1_HEAP_H

#include "linked_list.h"

typedef struct {
	uint32_t start_address;
	uint32_t size;
} heap_block_t;

typedef struct {
	uint32_t start_address;
	linked_list_t *blocks;
} heap_pool_t;

typedef struct {
	uint32_t start_address;
	linked_list_t *pools;
} heap_t;

heap_t create_heap(uint32_t start_address, uint32_t number_of_pools, uint32_t pool_size, uint32_t reconstruction_type);

#endif //TEMA1_HEAP_H
