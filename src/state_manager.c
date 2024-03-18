/*
Nume: Voinea Radu-Mihai
Grupa: 315 CA
*/

#include <string.h>
#include "api/state_manager.h"
#include "api/heap.h"
#include "api/utils.h"

static string_to_handle command_table[] = {
		{"save",      foo},
		{"init_heap", init_heap},
};

int process_command(string_t command)
{
	static heap_t *heap = NULL;

	if(heap==NULL){
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

int foo(int args_size, string_t *args, heap_t *heap)
{
	return CONTINUE;
}

int init_heap(int args_size, string_t *args, heap_t *heap)
{
	uint32_t start_address = strtol(args[1], NULL, 10);
	uint32_t number_of_pools = strtol(args[2], NULL, 10);
	uint32_t pool_total_size = strtol(args[3], NULL, 10);
	uint32_t reconstruction_type = strtol(args[4], NULL, 10);

	// TODO Free the old heap

	*heap = create_heap(start_address, number_of_pools, pool_total_size, reconstruction_type);

	return CONTINUE;
}
