/*
Nume: Voinea Radu-Mihai
Grupa: 315 CA
*/

#include "api/heap.h"
#include "api/utils.h"

heap_t create_heap(uint32_t start_address, uint32_t number_of_pools, uint32_t pool_total_size, uint32_t reconstruction_type)
{
	uint64_t address = start_address;

	heap_t *heap = (heap_t *)safe_malloc(sizeof(heap_t));

	heap->start_address = address;
	heap->pools = create_linked_list();

	uint32_t pool_size = 4;

	for (uint32_t i = 0; i < number_of_pools; i++) {
		pool_size *= 2;
		uint32_t blocks_size = pool_total_size / pool_size;

		heap_pool_t *pool = safe_malloc(sizeof(heap_pool_t));

		pool->start_address = address;
		pool->blocks = create_linked_list();

		for (uint32_t j = 0; j < blocks_size; j++) {
			heap_block_t *block = safe_malloc(sizeof(heap_block_t));

			block->start_address = address;
			block->size = pool_size;
			add_node(pool->blocks, block);

			address += pool_size;

			printf("Creating block at 0x%x with size %d\n", block->start_address, block->size);
		}
		printf("\n");

		add_node(heap->pools, pool);
	}

	return *heap;
}
