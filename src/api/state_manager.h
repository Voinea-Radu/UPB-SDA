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

	uint8_t (*handle)(int64_t args_size, string_t *args, heap_t *heap);
} string_to_handle;

uint8_t process_command(string_t command);

uint8_t handle_init_heap(int64_t args_size, string_t *args, heap_t *heap);

uint8_t handle_malloc(int64_t args_size, string_t *args, heap_t *heap);

uint8_t handle_dump(int64_t args_size, string_t *args, heap_t *heap);

uint8_t handle_free(int64_t args_size, string_t *args, heap_t *heap);

uint8_t handle_write(int64_t args_size, string_t *args, heap_t *heap);

uint8_t handle_read(int64_t args_size, string_t *args, heap_t *heap);

uint8_t handle_destroy(int64_t args_size, string_t *args, heap_t *heap);

void seg_fault(heap_t*heap);

#endif //TEMA1_STATE_MANAGER_H
