//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include "../utils/constants.h"

typedef struct queue_node {
	void *data;
	struct queue_node *next;
} queue_node_t;

typedef struct {
	queue_node_t *head;
	queue_node_t *tail;
	uint size;

	// Data functions
	bool (*data_compare)(void *data1, void *data2);
	uint (*data_get_size)(void *data);
	void (*data_free)(void **data);
} queue_t;

// ==================== Constructor(s) ====================

queue_t *queue_init(bool (*data_compare)(void *key1, void *key2), uint (*data_get_size)(void *data), void (*data_free)(void **data));

// ==================== Memory ====================

void queue_free(queue_t **queue);

void queue_node_free(queue_node_t **node, void (*data_free)(void **data));

// ==================== Functional ====================

void queue_enqueue(queue_t *queue, void *data);

void *queue_dequeue(queue_t *queue);

bool queue_is_empty(queue_t *queue);

void queue_remove(queue_t *queue, void *data);

#endif //QUEUE_H
