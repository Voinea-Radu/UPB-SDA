/*
 * Copyright (c) 2024 Coman Andrei-Madalin <andrei.coman1301@stud.acs.upb.ro>
 */

#include "queue.h"

queue_t *queue_create(size_t data_size, void *free_data_function) {
	queue_t *q = malloc(sizeof(*q));
	DIE(!q, "malloc failed");

	q->buff = dll_list_init(data_size, free_data_function);

	return q;
}

size_t queue_get_size(queue_t *q) {
	return dll_get_size(q->buff);
}

bool queue_is_empty(queue_t *q) {
	return dll_is_empty(q->buff);
}

void *queue_front(queue_t *q) {
	return dll_get_head(q->buff)->data;
}

dll_node_t *queue_dequeue(queue_t *q) {
	if (queue_is_empty(q))
		return NULL;

	dll_node_t *to_remove = dll_remove_head(q->buff);

	return to_remove;
}

void queue_enqueue(queue_t *q, void *new_data) {
	dll_node_t *new_node = dll_node_alloc();

	new_node->data = malloc(q->buff->data_size);
	memcpy(new_node->data, new_data, q->buff->data_size);

	dll_add_tail(q->buff, new_node);
}

void queue_free(queue_t *q) {
	dll_list_free(q->buff);

	free(q);
}
