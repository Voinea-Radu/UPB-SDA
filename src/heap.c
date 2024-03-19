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
	heap->used_blocks = create_linked_list();

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

			printf("Creating block at 0x%lx with size %lu\n", block->start_address, block->size);
		}
		printf("\n");

		add_node(heap->pools, pool);
	}

	return *heap;
}

void heap_add_block(heap_t *heap, heap_block_t *block)
{
	linked_list_t *pools_list = heap->pools;
	node_t *current_pool_node = pools_list->head;

	while (current_pool_node != NULL) {
		heap_pool_t *current_pool = (heap_pool_t *)current_pool_node->data;
		heap_pool_t *next_pool = current_pool_node->next == NULL ? NULL : (heap_pool_t *)current_pool_node->next->data;

		if (current_pool->block_size == block->size) {
			node_t *current_block_node = current_pool->blocks->head;
			while (current_block_node != NULL) {
				heap_block_t *current_block = (heap_block_t *)current_block_node->data;
				heap_block_t *next_block = current_pool_node->next == NULL ? NULL : (heap_block_t *)current_block_node->next->data;

				if (current_block->start_address < block->start_address && (next_block == NULL || block->start_address < next_block->start_address)) {
					node_t *new_node = safe_malloc(sizeof(node_t));

					node_t *next_node = current_block_node->next;

					new_node->data = block;
					new_node->next = current_block_node->next;
					new_node->prev = current_block_node;

					current_block_node->next = new_node;
					if(next_node != NULL){
						next_node->prev = new_node;
					}

					return;
				}

				current_block_node = current_block_node->next;
			}

			add_node(current_pool->blocks, block);
			return;
		}

		if (current_pool->block_size < block->size && (next_pool == NULL || block->size < next_pool->block_size)) {
			heap_pool_t *new_pool = safe_malloc(sizeof(heap_pool_t));

			new_pool->start_address = current_pool->start_address;
			new_pool->block_size = block->size;
			new_pool->blocks = create_linked_list();

			node_t *next_pool_node = current_pool_node->next;
			node_t *new_pool_node = safe_malloc(sizeof(node_t));

			new_pool_node->data = new_pool;
			new_pool_node->next = current_pool_node->next;
			new_pool_node->prev = current_pool_node;

			current_pool_node->next = new_pool_node;
			if(next_pool_node != NULL){
				next_pool_node->prev = new_pool_node;
			}

			return;
		}

		current_pool_node = current_pool_node->next;
	}
}

uint64_t heap_malloc(heap_t *heap, uint64_t size)
{
	linked_list_t *pools = heap->pools;
	linked_list_t *used_blocks = heap->used_blocks;

	node_t *current_pool = pools->head;

	while (current_pool != NULL) {
		heap_pool_t *pool = (heap_pool_t *)current_pool->data;

		if (pool->block_size < size) {
			current_pool = current_pool->next;
			continue;
		}

		linked_list_t *blocks = pool->blocks;
		node_t *current_block = blocks->head;

		while (current_block != NULL) {
			heap_block_t *block = (heap_block_t *)current_block->data;

			if (block->size == size) {
				add_node(used_blocks, block);
				remove_node(blocks, block);

				return block->start_address;
			}

			if (block->size > size) {
				heap_block_t *new_block = safe_malloc(sizeof(heap_block_t));
				new_block->start_address = block->start_address;
				new_block->size = size;

				add_node(used_blocks, new_block);
				remove_node(blocks, block);

				block->start_address += size;
				block->size -= size;
				heap_add_block(heap, block);

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

	printf("[Free] Heap at 0x%lx:\n", heap->start_address);
	while (pool_node != NULL) {
		heap_pool_t *pool = (heap_pool_t *)pool_node->data;
		printf("Pool at 0x%lx with block size %lu:\n", pool->start_address, pool->block_size);

		node_t *block_node = pool->blocks->head;

		while (block_node != NULL) {
			heap_block_t *block = (heap_block_t *)block_node->data;
			printf("\t- Block at 0x%lx with size %lu\n", block->start_address, block->size);

			block_node = block_node->next;
		}
		printf("\n");

		pool_node = pool_node->next;
	}
	printf("\n");

	node_t *used_block_node = heap->used_blocks->head;
	printf("[Used] Heap at 0x%lx:\n", heap->start_address);
	while (used_block_node != NULL) {
		heap_block_t *block = (heap_block_t *)used_block_node->data;
		printf("\t- Block at 0x%lx with size %lu\n", block->start_address, block->size);

		used_block_node = used_block_node->next;
	}

	printf("\n");
}
