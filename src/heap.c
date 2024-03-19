/*
Nume: Voinea Radu-Mihai
Grupa: 315 CA
*/

#include "api/heap.h"
#include "api/utils.h"

int8_t compare_blocks(void *data1, void *data2)
{
	heap_block_t *block1 = (heap_block_t *)data1;
	heap_block_t *block2 = (heap_block_t *)data2;

	if (block1->start_address == block2->start_address) {
		return 0;
	}

	return block1->start_address < block2->start_address ? -1 : 1;
}

int8_t compare_pools(void *data1, void *data2)
{
	heap_pool_t *pool1 = (heap_pool_t *)data1;
	heap_pool_t *pool2 = (heap_pool_t *)data2;

	if (pool1->block_size == pool2->block_size) {
		return 0;
	}

	return pool1->block_size < pool2->block_size ? -1 : 1;
}

heap_t *new_heap(int64_t start_address)
{
	heap_t *heap = safe_malloc(sizeof(heap_t));

	heap->start_address = start_address;
	heap->pools = create_linked_list(compare_pools);
	heap->used_blocks = create_linked_list(compare_blocks);

	return heap;
}

heap_pool_t *new_heap_pool(int64_t start_address, int64_t block_size)
{
	heap_pool_t *pool = safe_malloc(sizeof(heap_pool_t));

	pool->start_address = start_address;
	pool->block_size = block_size;
	pool->blocks = create_linked_list(compare_blocks);

	return pool;
}

heap_block_t *new_heap_block(int64_t start_address, int64_t size)
{
	heap_block_t *block = safe_malloc(sizeof(heap_block_t));

	block->start_address = start_address;
	block->size = size;

	return block;
}

heap_t create_heap(int64_t start_address, int64_t number_of_pools, int64_t pool_total_size, bool reconstruction_type)
{
	heap_t *heap = new_heap(start_address);

	int64_t address = start_address;
	int64_t pool_size = 1;

	for (int64_t i = 0; i < number_of_pools; i++) {
		int64_t blocks_size = pool_total_size / pool_size;
		heap_pool_t *pool = new_heap_pool(address, pool_size);

		for (int64_t j = 0; j < blocks_size; j++) {
			heap_block_t *block = new_heap_block(address, pool_size);
			add_node_at_tail(pool->blocks, block);

			address += pool_size * 8;

			printf("Creating block at 0x%lx with size of %lu byte(s)\n", block->start_address, block->size);
		}
		printf("\n");

		add_node_at_tail(heap->pools, pool);
		pool_size *= 2;
	}

	return *heap;
}

void heap_add_block(heap_t *heap, heap_block_t *block)
{
	linked_list_t *pools_list = heap->pools;
	node_t *current_pool_node = pools_list->head;

	while (current_pool_node != NULL) {
		heap_pool_t *current_pool = (heap_pool_t *)current_pool_node->data;

		if (current_pool->block_size == block->size) {
			add_node(current_pool->blocks, block);
			return;
		}

		current_pool_node = current_pool_node->next;
	}

	heap_pool_t *pool = new_heap_pool(block->start_address, block->size);
	add_node(pools_list, pool);

	add_node(pool->blocks, block);
}

int64_t heap_malloc(heap_t *heap, int64_t size)
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
				heap_block_t *new_block = new_heap_block(block->start_address, size);

				add_node(used_blocks, new_block);
				remove_node(blocks, block);

				block->start_address += size;
				block->size -= size;
				heap_add_block(heap, block);

				return block->start_address;
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
