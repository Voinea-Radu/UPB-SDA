/*
 * Copyright (c) 2024 Coman Andrei-Madalin <andrei.coman1301@stud.acs.upb.ro>
 */

#ifndef STACK_H
#define STACK_H

#include "double_linked_list.h"

typedef struct {
	double_linked_list_t *list;
} stack_t;

stack_t *stack_init(size_t data_size, void (*free_data_function)(void *));

void stack_push(stack_t *stack, void *data);

// TODO: review this free
void *stack_pop(stack_t *stack);

bool stack_is_empty(stack_t *stack);

void stack_free(stack_t *stack);

#endif /* STACK_H */
