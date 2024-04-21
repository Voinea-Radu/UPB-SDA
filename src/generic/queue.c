//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include "../generic/queue.h"
#include "../utils/utils.h"

queue_t *queue_init()
{
	queue_t *queue = safe_malloc(sizeof(queue_t));

	queue->head = NULL;
	queue->tail = NULL;
	queue->size = 0;

	return queue;
}

bool queue_enqueue(queue_t *queue, void *data)
{
	queue_node_t *node = safe_malloc(sizeof(queue_node_t));
	node->data = data;
	node->next = NULL;

	if (queue->size == 0) {
		queue->head = node;
		queue->tail = node;
	} else {
		queue->tail->next = node;
		queue->tail = node;
	}

	queue->size++;

	return true;
}

void *queue_dequeue(queue_t *queue)
{
	if (queue->size == 0) {
		return NULL;
	}

	queue_node_t *node = queue->head;
	void *data = node->data;

	queue->head = node->next;
	queue->size--;

	free(node);

	return data;
}

bool queue_is_empty(queue_t *queue)
{
	return queue->size == 0;
}

void queue_free(queue_t **queue)
{
	queue_node_t *node = (*queue)->head;
	queue_node_t *next;

	while (node) {
		next = node->next;
		free(node);
		node = next;
	}

	free(*queue);
	*queue = NULL;
}

void queue_print(queue_t *queue, string_t (*to_string_function)(void *), string_t prefix)
{
	queue_node_t *node = queue->head;

	while (node) {
		printf("%s%s\n", prefix, to_string_function(node->data));
		node = node->next;
	}
}

