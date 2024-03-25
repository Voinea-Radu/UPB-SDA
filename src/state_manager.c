/*
Nume: Voinea Radu-Mihai
Grupa: 315 CA
*/

#include <string.h>
#include "api/state_manager.h"
#include "api/heap.h"
#include "api/utils.h"

static string_to_handle command_table[] = {
		{"init_heap",    handle_init_heap},
		{"malloc",       handle_malloc},
		{"dump",         handle_dump},
		{"dump_memory",  handle_dump},
		{"free",         handle_free},
		{"write",        handle_write},
		{"read",         handle_read},
		{"destroy_heap", handle_destroy},
};

uint8_t process_command(string_t command)
{
	static heap_t *heap = NULL;

	if (heap == NULL) {
		heap = safe_malloc(sizeof(heap_t));
	}

	static const size_t commands_size = sizeof(command_table) / sizeof(string_to_handle);

	int output = UNKNOWN_COMMAND;

	int args_size = 0;
	string_t *args = split_string(&args_size, command, ' ');

	to_lower(args[0]);

	for (size_t i = 0; i < commands_size; i++) {
		string_to_handle pair = command_table[i];

		if (strcmp(pair.key, args[0]) == 0)
			output = pair.handle(args_size, args, heap);
	}

	if (UNKNOWN_COMMAND == output)
		printf("Invalid command\n");

	for (int i = 0; i < args_size; i++) {
		free(args[i]);
	}
	free(args);

	return output;
}

uint8_t handle_init_heap(int64_t args_size, string_t *args, heap_t *heap)
{
	int64_t start_address = strtol(args[1], NULL, 16);
	int64_t number_of_pools = strtol(args[2], NULL, 10);
	int64_t pool_total_size = strtol(args[3], NULL, 10);
	int64_t reconstruction_type = strtol(args[4], NULL, 10);

	// TODO Free the old heap

	*heap = create_heap(start_address, number_of_pools, pool_total_size, reconstruction_type);

	return CONTINUE;
}

uint8_t handle_malloc(int64_t args_size, string_t *args, heap_t *heap)
{
	int64_t size = strtol(args[1], NULL, 10);
	int64_t block_address = heap_malloc(heap, size);

	if (block_address == 0xFFFFFFFFFFFFFFFF) { // Comparison with -1 is marked as a warning in CLion.
		printf("Out of memory\n");
	}
	//else {
	//	printf("Allocated memory at address 0x%lx\n", block_address * 8);
	//}

	return CONTINUE;
}

uint8_t handle_dump(int64_t args_size, string_t *args, heap_t *heap)
{
	dump_heap(heap);
	return CONTINUE;
}

uint8_t handle_free(int64_t args_size, string_t *args, heap_t *heap)
{
	int64_t start_address = strtol(args[1], NULL, 16);

	bool success = heap_free(heap, start_address);

	if (!success) {
		printf("Invalid free\n");
	}

	return CONTINUE;
}

uint8_t handle_write(int64_t args_size, string_t *args, heap_t *heap)
{
	int64_t start_address = strtol(args[1], NULL, 16);

	string_t value = malloc(sizeof(char));
	value[0] = '\0';
	int64_t value_size = 0;

	for (int i = 2; i < args_size - 1; i++) {
		value_size += strlen(args[i]) + 1;
		value = safe_realloc(value, value_size * sizeof(char));
		strcat(value, args[i]);

		if (i != args_size - 2) {
			value_size++;
			value = safe_realloc(value, value_size * sizeof(char));
			strcat(value, " ");
		}
	}
	int64_t write_size = strtol(args[args_size - 1], NULL, 10);

	if (value[0] != '"' || value[strlen(value) - 1] != '"') {
		printf("Invalid write\n");
		return CONTINUE;
	}

	for (int i = 0; i < strlen(value); i++) {
		value[i] = value[i + 1];
	}

	value[strlen(value) - 1] = '\0';

	bool success = heap_write(heap, start_address, write_size, value);
	free(value);

	if (!success) {
		seg_fault(heap);
		return EXIT;
	}

	return CONTINUE;
}

void seg_fault(heap_t *heap)
{
	printf("Segmentation fault (core dumped)\n");
	handle_dump(0, NULL, heap);
}

uint8_t handle_read(int64_t args_size, string_t *args, heap_t *heap)
{
	int64_t start_address = strtol(args[1], NULL, 16);
	int64_t size = strtol(args[2], NULL, 10);

	string_t value = heap_read(heap, start_address, size);

	if (value == NULL || strcmp(value, "") == 0) {
		seg_fault(heap);
		free(value);
		return EXIT;
	}

	printf("%s\n", value);
	free(value);
	return CONTINUE;
}

uint8_t handle_destroy(int64_t args_size, string_t *args, heap_t *heap)
{
	heap_destroy(heap);
	return EXIT;
}