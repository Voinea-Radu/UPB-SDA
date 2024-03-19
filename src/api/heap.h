/*
Nume: Voinea Radu-Mihai
Grupa: 315 CA
*/

#ifndef TEMA1_HEAP_H
#define TEMA1_HEAP_H

#include <stdbool.h>
#include "linked_list.h"
#include "string_utils.h"

typedef struct {
	int64_t holding_block_start_address; // in bytes

	int64_t address; // in bytes
	char data;
} heap_byte_t;

typedef struct {
	int64_t start_address; // in bytes
	int64_t size; // in bytes
} heap_block_t;

typedef struct {
	int64_t start_address; // in bytes
	int64_t block_size; // in bytes
	linked_list_t *blocks;
} heap_pool_t;

typedef struct {
	int64_t start_address; // in bytes
	linked_list_t *pools;
	linked_list_t *bytes;
} heap_t;

heap_t *new_heap(int64_t start_address);

heap_pool_t *new_heap_pool(int64_t start_address, int64_t block_size);

heap_block_t *new_heap_block(int64_t start_address, int64_t size);

heap_byte_t *new_heap_byte(int64_t holding_block_start_address, int64_t offset);

heap_byte_t **new_heap_bytes(heap_block_t *block);

int8_t compare_blocks(void *data1, void *data2);

int8_t compare_pools(void *data1, void *data2);

int8_t compare_bytes(void *data1, void *data2);

heap_t create_heap(int64_t start_address, int64_t number_of_pools, int64_t pool_size, bool reconstruction_type);

void heap_add_block(heap_t *heap, heap_block_t *block);

int64_t heap_malloc(heap_t *heap, int64_t size);

void dump_heap(heap_t *heap);

bool heap_free(heap_t *heap, int64_t start_address);

bool heap_write(heap_t *heap, int64_t start_address, int64_t size, string_t data);

bool heap_read(heap_t *heap, int64_t start_address, int64_t size);

#endif //TEMA1_HEAP_H
