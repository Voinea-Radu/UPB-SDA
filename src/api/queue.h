/*
 * Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
 */

#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

typedef struct queue_node {
	void *data;
	struct queue_node *next;
} queue_node_t;

typedef struct {
	queue_node_t *head;
	queue_node_t *tail;
	unsigned int size;
} queue_t;

queue_t *queue_init();

bool queue_enqueue(queue_t *queue, void *data);

void *queue_dequeue(queue_t *queue);

void queue_free(queue_t **queue);

#endif //QUEUE_H
