/*
Nume: Voinea Radu-Mihai
Grupa: 315 CA
*/

#include "api/heap.h"
#include "api/utils.h"

heap_t create_heap(uint64_t start_address, uint64_t number_of_pools, uint64_t pool_total_size, uint64_t reconstruction_type)
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
		pool->block_size = pool_size;
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

uint64_t heap_malloc(heap_t *heap, uint64_t size)
{
	linked_list_t *pools = heap->pools;
	linked_list_t *used_blocks = heap->used_blocks;

	node_t *current_pool = pools->head;

	while (current_pool != NULL) {
		heap_pool_t *pool = (heap_pool_t *)current_pool->data;

		if(pool->block_size < size) {
			current_pool = current_pool->next;
			continue;
		}

		linked_list_t *blocks = pool->blocks;
		node_t *current_block = blocks->head;

		while (current_block != NULL) {
			heap_block_t *block = (heap_block_t *)current_block->data;

			if (block->size >= size) {
				heap_block_t *new_block = safe_malloc(sizeof(heap_block_t));
				new_block->start_address = block->start_address;
				new_block->size = size;

				add_node(used_blocks, new_block);

				block->start_address += size;
				block->size -= size;

				return new_block->start_address;
			}

			current_block = current_block->next;
		}

		current_pool = current_pool->next;
	}

	return 0;
}

void dump_heap(heap_t *heap)
{
	node_t *pool_node = heap->pools->head;

	while(pool_node != NULL){
		heap_pool_t *pool = (heap_pool_t *)pool_node->data;
		printf("Pool at 0x%lx with block size %lu:\n", pool->start_address, pool->block_size);

		node_t *block_node = pool->blocks->head;

		while(block_node != NULL){
			heap_block_t *block = (heap_block_t *)block_node->data;
			printf("\t- Block at 0x%lx with size %lu\n", block->start_address, block->size);

			block_node = block_node->next;
		}
		printf("\n");

		pool_node = pool_node->next;
	}
}