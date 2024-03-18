/*
Nume: Voinea Radu-Mihai
Grupa: 315 CA
*/

#ifndef TEMA1_STATE_MANAGER_H
#define TEMA1_STATE_MANAGER_H

#include "string_utils.h"
#include "heap.h"

#define EXIT 0
#define CONTINUE 1
#define UNKNOWN_COMMAND 2

typedef struct {
	string_t key;

	int (*handle)(int args_size, string_t *args, heap_t *heap);
} string_to_handle;

int process_command(string_t command);

int foo(int args_size, string_t *args, heap_t *heap);
int init_heap(int args_size, string_t *args, heap_t *heap);

#endif //TEMA1_STATE_MANAGER_H
