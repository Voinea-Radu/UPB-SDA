/*
Nume: Voinea Radu-Mihai
Grupa: 315 CA
*/

#include <string.h>
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

	bool _tmp = (block1->start_address < block2->start_address);

	return (_tmp) ? -1 : 1;
}

/**
 * Compare two bytes by their address
 * @param data1 This should be of instance heap_byte_t
 * @param data2 This should be of instance heap_byte_t
 * @return -1 if data1 < data2, 0 if data1 == data2, 1 if data1 > data2
 */
int8_t compare_bytes(void *data1, void *data2)
{
	heap_byte_t *byte1 = (heap_byte_t *)data1;
	heap_byte_t *byte2 = (heap_byte_t *)data2;

	if (byte1->address == byte2->address) {
		return 0;
	}

	return byte1->address < byte2->address ? -1 : 1;
}

heap_byte_t *new_heap_byte(int64_t holding_block_start_address, int64_t offset)
{
	heap_byte_t *byte = safe_malloc(sizeof(heap_byte_t));

	byte->holding_block_start_address = holding_block_start_address;
	byte->address = holding_block_start_address + offset;
	byte->data = 0;

	return byte;
}

heap_byte_t **new_heap_bytes(heap_block_t *block)
{
	heap_byte_t **bytes = safe_malloc(block->size * sizeof(heap_byte_t *));

	for (int64_t i = 0; i < block->size; i++) {
		bytes[i] = new_heap_byte(block->start_address, i);
	}

	return bytes;
}

heap_t new_heap(int64_t start_address, int64_t number_of_pools, int64_t max_pool_size)
{
	heap_t heap;

	heap.start_address = start_address;

	heap.pools_size = min(power_of_two(number_of_pools + 2), max_pool_size) + 1;
	heap.pools = safe_malloc(heap.pools_size * sizeof(linked_list_t*));

	for (int64_t i = 0; i < heap.pools_size; i++) {
		heap.pools[i] = new_linked_list(compare_blocks);
	}

	heap.bytes = new_linked_list(compare_bytes);

	heap.malloc_calls_count = 0;
	heap.free_calls_count = 0;
	heap.fragmentation_count = 0;

	return heap;
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
	heap_t heap = new_heap(start_address, number_of_pools, pool_total_size);

	int64_t address = start_address;
	int64_t pool_size = 8;

	for (int64_t i = 0; i < number_of_pools; i++) {
		int64_t blocks_size = pool_total_size / pool_size;

		for (int64_t j = 0; j < blocks_size; j++) {
			heap_block_t *block = new_heap_block(address, pool_size);
			add_node_at_tail(heap.pools[pool_size], block);

			address += pool_size;
			//printf("Creating block at 0x%lx with size of %lu byte(s)\n", block->start_address * 8, block->size);
		}
		//printf("\n");

		pool_size *= 2;
	}

	return heap;
}

void heap_add_block(heap_t *heap, heap_block_t *block)
{
	linked_list_t *pool = heap->pools[block->size];
	add_node_sorted(pool, block);
}

/**
 * Get a block of a certain size from the heap. The block returned can be larger than the requested size.
 * @param heap The heap to search in
 * @param size The size of the block
 * @return The heap_block_t instance of the block or NULL if no block was found
 */
heap_block_t *heap_get_block_of_size(heap_t *heap, int64_t size)
{
	if(size >= heap->pools_size)
		return NULL;

	linked_list_t *exact_size_pool = heap->pools[size];

	if (exact_size_pool->size > 0) {
		return (heap_block_t *)exact_size_pool->head->data;
	}

	for (int64_t i = size + 1; i < heap->pools_size; i++) {
		linked_list_t *pool = heap->pools[i];

		if (pool->size > 0) {
			return (heap_block_t *)pool->head->data;
		}
	}

	return NULL;
}

void heap_remove_block(heap_t *heap, heap_block_t *block)
{
	linked_list_t *pool = heap->pools[block->size];

	node_t *removed_block_node = remove_node(pool, block);
	heap_block_t *removed_block = (heap_block_t *)removed_block_node->data;

	free(removed_block_node);
	free(removed_block);
}


int64_t heap_malloc(heap_t *heap, int64_t size)
{
	heap_block_t *current_block = heap_get_block_of_size(heap, size);

	if (current_block != NULL) {
		if (current_block->size == size) {
			heap_byte_t **bytes = new_heap_bytes(current_block);

			for (int64_t i = 0; i < current_block->size; i++) {
				add_node_sorted(heap->bytes, bytes[i]);
			}

			int64_t address = current_block->start_address;

			heap_remove_block(heap, current_block);
			free(bytes);

			heap->malloc_calls_count++;
			return address;
		}

		heap->fragmentation_count++;

		heap_block_t *new_block = new_heap_block(current_block->start_address, size);
		int64_t address = new_block->start_address;

		heap_byte_t **bytes = new_heap_bytes(new_block);

		for (int64_t i = 0; i < new_block->size; i++) {
			add_node_sorted(heap->bytes, bytes[i]);
		}

		free(bytes);

		heap_block_t *remaining_block = new_heap_block(current_block->start_address + size, current_block->size - size);
		heap_add_block(heap, remaining_block);
		heap_remove_block(heap, current_block);
		free(new_block);

		heap->malloc_calls_count++;
		return address;
	}

	return -1;
}

void dump_heap(heap_t *heap)
{
	int64_t free_memory = heap_get_free_size(heap);
	int64_t used_memory = heap->bytes->size;
	int64_t total_memory = free_memory + used_memory;

	printf("+++++DUMP+++++\n");
	printf("Total memory: %ld bytes\n", total_memory);
	printf("Total allocated memory: %ld bytes\n", used_memory);
	printf("Total free memory: %ld bytes\n", free_memory);
	printf("Free blocks: %ld\n", heap_get_free_blocks_count(heap));
	printf("Number of allocated blocks: %ld\n", heap_get_allocated_blocks_count(heap));
	printf("Number of malloc calls: %ld\n", heap->malloc_calls_count);
	printf("Number of fragmentations: %ld\n", heap->fragmentation_count);
	printf("Number of free calls: %ld\n", heap->free_calls_count);

	for (int64_t i = 0; i < heap->pools_size; i++){
		linked_list_t *pool = heap->pools[i];

		if (pool->size == 0) {
			continue;
		}

		printf("Blocks with %lu bytes - %lu free block(s) :", i, pool->size);

		node_t *block_node = pool->head;

		while (block_node != NULL) {
			heap_block_t *block = (heap_block_t *)block_node->data;
			printf(" 0x%lx", block->start_address);

			block_node = block_node->next;
		}
		printf("\n");
	}

	printf("Allocated blocks :");
	node_t *used_byte_node = heap->bytes->head;

	int64_t size = 1;
	int64_t last_address = -1;

	while (used_byte_node != NULL) {
		heap_byte_t *byte = (heap_byte_t *)used_byte_node->data;

		if (byte->holding_block_start_address == last_address) {
			size++;
		} else {
			if (last_address != -1) {
				printf(" (0x%lx - %ld)", last_address, size);
			}
			last_address = byte->holding_block_start_address;
			size = 1;
		}

		used_byte_node = used_byte_node->next;
	}

	if (last_address != -1) {
		printf(" (0x%lx - %ld)", last_address, size);
	}

	printf("\n");
	printf("-----DUMP-----\n");
}

int64_t heap_get_free_size(heap_t *heap)
{
	int64_t size = 0;

	for (int i = 0; i < heap->pools_size; i++){
		linked_list_t *pool = heap->pools[i];
		size+=pool->size * i;
	}

	return size;

}

bool heap_free(heap_t *heap, int64_t start_address)
{
	node_t *used_block_node = heap->bytes->head;
	node_t *next_used_block_node;
	int64_t size = 0;

	while (used_block_node != NULL) {
		next_used_block_node = used_block_node->next;
		heap_byte_t *byte = (heap_byte_t *)used_block_node->data;

		if (byte->holding_block_start_address == start_address) {
			node_t *removed_byte_node = remove_node(heap->bytes, byte);
			heap_byte_t *removed_byte = (heap_byte_t *)removed_byte_node->data;

			free(removed_byte_node);
			free(removed_byte);

			size++;
		}

		used_block_node = next_used_block_node;
	}

	if (size != 0) {
		heap_add_block(heap, new_heap_block(start_address, size));
		heap->free_calls_count++;
	}

	return size != 0;
}

bool heap_write(heap_t *heap, int64_t start_address, int64_t size, string_t data)
{
	size = min(size, strlen(data));

	node_t *used_block_node = heap->bytes->head;
	node_t *search_start_block_node;

	while (used_block_node != NULL) {
		heap_byte_t *block = (heap_byte_t *)used_block_node->data;

		if (block->address == start_address) {
			search_start_block_node = used_block_node;
			for (int i = 0; i < size - 1; i++) {
				used_block_node = used_block_node->next;

				if (used_block_node == NULL) {
					return false;
				}
			}

			heap_byte_t *found_byte = (heap_byte_t *)used_block_node->data;

			if (found_byte->address == start_address + size - 1) {
				for (int i = 0; i < size; i++) {
					heap_byte_t *byte = (heap_byte_t *)search_start_block_node->data;
					byte->data = data[i];
					search_start_block_node = search_start_block_node->next;
				}
				return true;
			}
		}

		used_block_node = used_block_node->next;
	}

	return false;
}

string_t heap_read(heap_t *heap, int64_t start_address, int64_t size)
{
	string_t result = safe_malloc(size * sizeof(char) + 1);
	node_t *used_block_node = heap->bytes->head;
	int64_t current_address = start_address;
	int64_t current_size = 0;

	while (used_block_node != NULL) {
		heap_byte_t *byte = (heap_byte_t *)used_block_node->data;

		if (byte->address == current_address) {
			result[current_size++] = byte->data;
			current_address++;
		} else if(current_size!=0){
			free(result);
			return NULL;
		}

		if (current_size == size)
			break;

		used_block_node = used_block_node->next;
	}

	result[current_size] = '\0';

	return result;
}

int64_t heap_get_allocated_blocks_count(heap_t *heap)
{
	int64_t count = 0;
	node_t *byte_node = heap->bytes->head;
	int64_t last_address = -1;

	while (byte_node != NULL) {
		heap_byte_t *pool = (heap_byte_t *)byte_node->data;

		if (pool->holding_block_start_address != last_address) {
			count++;
			last_address = pool->holding_block_start_address;
		}

		byte_node = byte_node->next;
	}
	return count;
}

int64_t heap_get_free_blocks_count(heap_t *heap)
{
	int64_t count = 0;

	for (int i = 0; i < heap->pools_size; i++){
		linked_list_t *pool = heap->pools[i];
		count+=pool->size;
	}

	return count;
}

void heap_destroy(heap_t *heap)
{
	for (int i = 0; i < heap->pools_size; i++) {
		linked_list_t *pool = heap->pools[i];
		node_t *block_node = pool->head;
		node_t *next_block_node;

		while (block_node != NULL) {
			next_block_node = block_node->next;

			heap_block_t *block = (heap_block_t *)block_node->data;
			free(block);
			free(block_node);

			block_node = next_block_node;
		}

		free(pool);
	}

	node_t *byte_node = heap->bytes->head;
	node_t *next_byte_node;

	while (byte_node != NULL) {
		next_byte_node = byte_node->next;

		heap_byte_t *byte = (heap_byte_t *)byte_node->data;

		free(byte);
		free(byte_node);

		byte_node = next_byte_node;
	}

	free(heap->bytes);
	free(heap->pools);

	free(heap);
}
