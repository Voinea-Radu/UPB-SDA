/*
Nume: Voinea Radu-Mihai
Grupa: 315 CA
*/

#include "api/heap.h"
#include "api/utils.h"

/**
 * Compare two blocks by their start address
 * @param data1 This should be of instance heap_block_t
 * @param data2 This should be of instance heap_block_t
 * @return -1 if data1 < data2, 0 if data1 == data2, 1 if data1 > data2
 */
int8_t compare_blocks(void *data1, void *data2)
{
	heap_block_t *block1 = (heap_block_t *)data1;
	heap_block_t *block2 = (heap_block_t *)data2;

	if (block1->start_address == block2->start_address) {
		return 0;
	}

	bool _tmp =  (block1->start_address < block2->start_address);

	return (_tmp) ? -1 : 1;
}

/**
 * Compare two pools by their block size
 * @param data1 This should be of instance heap_pool_t
 * @param data2 This should be of instance heap_pool_t
 * @return -1 if data1 < data2, 0 if data1 == data2, 1 if data1 > data2
 */
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

			address += pool_size;

			printf("Creating block at 0x%lx with size of %lu byte(s)\n", block->start_address*8, block->size);
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

/**
 * Get a block of a certain size from the heap. The block returned can be larger than the requested size.
 * @param heap The heap to search in
 * @param size The size of the block
 * @return The heap_block_t instance of the block or NULL if no block was found
 */
heap_block_t *heap_get_block_of_size(heap_t *heap, int64_t size)
{
	linked_list_t *pools = heap->pools;
	node_t *current_pool = pools->head;

	while (current_pool != NULL) {
		heap_pool_t *pool = (heap_pool_t *)current_pool->data;
		linked_list_t *blocks = pool->blocks;

		if (blocks->head == NULL) {
			current_pool = current_pool->next;
			continue;
		}

		heap_block_t *head_block = (heap_block_t *)blocks->head->data;

		if (pool->block_size < size) {
			// no-op
		} else if (pool->block_size >= size) {
			if (head_block != NULL) {
				return head_block;
			}
		}

		current_pool = current_pool->next;
	}

	return NULL;
}

void heap_remove_block(heap_t *heap, heap_block_t *block)
{
	linked_list_t *pools_list = heap->pools;
	node_t *current_pool_node = pools_list->head;

	while (current_pool_node != NULL) {
		heap_pool_t *current_pool = (heap_pool_t *)current_pool_node->data;

		if (current_pool->block_size == block->size) {
			remove_node(current_pool->blocks, block);
			return;
		}

		current_pool_node = current_pool_node->next;
	}
}

int64_t heap_malloc(heap_t *heap, int64_t size)
{
	heap_block_t *current_block = heap_get_block_of_size(heap, size);

	if (current_block != NULL) {
		if (current_block->size == size) {
			heap_remove_block(heap, current_block);
			add_node(heap->used_blocks, current_block);

			return current_block->start_address;
		}

		heap_block_t *new_block = new_heap_block(current_block->start_address, size);
		add_node(heap->used_blocks, new_block);
		heap_remove_block(heap, current_block);

		heap_block_t *remaining_block = new_heap_block(current_block->start_address + size, current_block->size - size);
		heap_add_block(heap, remaining_block);

		return new_block->start_address;
	}

	return -1;
}

void dump_heap(heap_t *heap)
{
	node_t *pool_node = heap->pools->head;

	printf("[Free] Heap at 0x%lx:\n", heap->start_address);
	while (pool_node != NULL) {
		heap_pool_t *pool = (heap_pool_t *)pool_node->data;
		printf("Pool at 0x%lx with block size %lu:\n", pool->start_address * 8, pool->block_size);

		node_t *block_node = pool->blocks->head;

		while (block_node != NULL) {
			heap_block_t *block = (heap_block_t *)block_node->data;
			printf("\t- Block at 0x%lx with size %lu\n", block->start_address * 8, block->size);

			block_node = block_node->next;
		}
		printf("\n");

		pool_node = pool_node->next;
	}
	printf("\n");

	node_t *used_block_node = heap->used_blocks->head;
	printf("[Used] Heap at 0x%lx:\n", heap->start_address * 8);
	while (used_block_node != NULL) {
		heap_block_t *block = (heap_block_t *)used_block_node->data;
		printf("\t- Block at 0x%lx with size %lu\n", block->start_address * 8, block->size);

		used_block_node = used_block_node->next;
	}

	printf("\n");
}

bool heap_free(heap_t *heap, int64_t start_address)
{
	node_t *used_block_node = heap->used_blocks->head;

	while (used_block_node != NULL) {
		heap_block_t *block = (heap_block_t *)used_block_node->data;

		if (block->start_address == start_address) {
			remove_node(heap->used_blocks, block);
			heap_add_block(heap, block);

			return true;
		}

		used_block_node = used_block_node->next;
	}

	return false;
}