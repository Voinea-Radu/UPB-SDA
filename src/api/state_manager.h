/*
Nume: Voinea Radu-Mihai
Grupa: 315 CA
*/

#ifndef TEMA1_STATE_MANAGER_H
#define TEMA1_STATE_MANAGER_H

#include "string_utils.h"
#include "heap.h"

#define FINISH 0
#define CONTINUE 1
#define UNKNOWN_COMMAND 2

typedef struct {
	string_t key;

	u8 (*handle)(s64 args_size, string_t *args, heap_t *heap);
} string_to_handle;

u8 process_command(string_t command);

u8 handle_init_heap(s64 args_size, string_t *args, heap_t *heap);

u8 handle_malloc(s64 args_size, string_t *args, heap_t *heap);

u8 handle_dump(s64 args_size, string_t *args, heap_t *heap);

u8 handle_free(s64 args_size, string_t *args, heap_t *heap);

u8 handle_write(s64 args_size, string_t *args, heap_t *heap);

u8 handle_read(s64 args_size, string_t *args, heap_t *heap);

u8 handle_destroy(s64 args_size, string_t *args, heap_t *heap);

void seg_fault(heap_t *heap);

#endif //TEMA1_STATE_MANAGER_H
