/*
Nume: Voinea Radu-Mihai
Grupa: 315 CA
*/

#ifndef TEMA1_HEAP_H
#define TEMA1_HEAP_H

#include <stdbool.h>
#include "linked_list.h"
#include "string_utils.h"
#include "utils.h"

typedef struct {
	s64 holding_block_start_address; // in bytes

	s8 data;
} heap_byte_t;

typedef struct {
	s64 start_address; // in bytes
	s64 size; // in bytes
} heap_block_t;

typedef struct {
	s64 start_address; // in bytes

	linked_list_t *(*pools); // array<linked_list_t*>
	s64 pools_size;

	heap_byte_t *(*bytes); // array<heap_byte_t*>
	s64 bytes_size;

	// Statistics
	s64 malloc_calls_count;
	s64 free_calls_count;
	s64 fragmentation_count;
} heap_t;

heap_t new_heap(s64 start_address, s64 number_of_pools, s64 max_pool_size);

heap_block_t *new_heap_block(s64 start_address, s64 size);

heap_byte_t *new_heap_byte(s64 holding_block_start_address);

s8 compare_blocks(void *data1, void *data2);

heap_t create_heap(s64 start_address, s64 number_of_pools, s64 pool_size,
				   __attribute__((unused)) bool reconstruction_type);

void heap_add_block(heap_t *heap, heap_block_t *block);

s64 heap_malloc(heap_t *heap, s64 size);

void dump_heap(heap_t *heap);

bool heap_free(heap_t *heap, s64 start_address);

bool heap_write(heap_t *heap, s64 start_address, s64 size, string_t data);

string_t heap_read(heap_t *heap, s64 start_address, s64 size);

s64 heap_get_free_size(heap_t *heap);

s64 heap_get_allocated_blocks_count(heap_t *heap);

s64 heap_get_free_blocks_count(heap_t *heap);

void heap_destroy(heap_t *heap);

s64 heap_get_used_size(heap_t *heap);

#endif //TEMA1_HEAP_H
