/*
Nume: Voinea Radu-Mihai
Grupa: 315 CA
*/

#include <string.h>
#include "api/state_manager.h"
#include "api/heap.h"
#include "api/utils.h"

static string_to_handle command_table[] = {
		{"init_heap", handle_init_heap},
		{"malloc",    handle_malloc},
		{"dump",      handle_dump},
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

	return output;
}

uint8_t handle_init_heap(uint64_t args_size, string_t *args, heap_t *heap)
{
	uint64_t start_address = strtol(args[1], NULL, 10);
	uint64_t number_of_pools = strtol(args[2], NULL, 10);
	uint64_t pool_total_size = strtol(args[3], NULL, 10);
	uint64_t reconstruction_type = strtol(args[4], NULL, 10);

	// TODO Free the old heap

	*heap = create_heap(start_address, number_of_pools, pool_total_size, reconstruction_type);

	return CONTINUE;
}

uint8_t handle_malloc(uint64_t args_size, string_t *args, heap_t *heap)
{
	uint64_t size = strtol(args[1], NULL, 10);

	uint64_t block_address = heap_malloc(heap, size);

	return CONTINUE;
}

uint8_t handle_dump(uint64_t args_size, string_t *args, heap_t *heap)
{
	dump_heap(heap);
	return CONTINUE;
}
