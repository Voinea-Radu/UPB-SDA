/*
 * Copyright (c) 2024 Coman Andrei-Madalin <andrei.coman1301@stud.acs.upb.ro>
 */

#include "stack.h"

stack_t *stack_init(size_t data_size, void (*free_data_function)(void *)) {
	stack_t *stack = malloc(sizeof(*stack));
	DIE(!stack, "malloc() failed");

	stack->list = dll_list_init(data_size, free_data_function);

	return stack;
}

void stack_push(stack_t *stack, void *data) {
	dll_add_head(stack->list, data);
}

void *stack_pop(stack_t *stack) {
	dll_node_t *node = dll_remove_head(stack->list);
	void *data = node->data;
	free(node);
	return data;
}

bool stack_is_empty(stack_t *stack) {
	return dll_is_empty(stack->list);
}

void stack_free(stack_t *stack) {
	dll_list_free(stack->list);
	free(stack);
}
