//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include "../generic/queue.h"
#include "../utils/utils.h"
queue_t *queue_init(bool (*data_compare)(void *key1, void *key2), uint (*data_get_size)(void *data), void (*data_free)(void **data))
{
	queue_t *queue = safe_malloc(sizeof(queue_t));

	queue->head = NULL;
	queue->tail = NULL;
	queue->size = 0;

	queue->data_compare = data_compare;
	queue->data_get_size = data_get_size;
	queue->data_free = data_free;

	return queue;
}

bool queue_enqueue(queue_t *queue, void *data)
{
	queue_node_t *node = safe_malloc(sizeof(queue_node_t));
	node->data = create_and_copy(data, queue->data_get_size);
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

	if (queue->size == 0) {
		queue->tail = NULL;
	}

	free(node);

	return data;
}

bool queue_is_empty(queue_t *queue)
{
	return queue->size == 0;
}

void queue_node_free(queue_node_t **node, void (*data_free)(void **data)){
	if(*node){
		data_free(&(*node)->data);
		free(*node);
		*node = NULL;
	}
}


void queue_free(queue_t **queue)
{
	queue_node_t *node = (*queue)->head;
	queue_node_t *next;

	while (node) {
		next = node->next;
		queue_node_free(&node, (*queue)->data_free);
		node = next;
	}

	free(*queue);
	*queue = NULL;
}

bool queue_remove(queue_t *queue, void *data)
{
	queue_node_t *node = queue->head;
	queue_node_t *prev = NULL;

	while (node) {
		if (queue->data_compare(node->data, data)) {
			if (prev) {
				prev->next = node->next;
			} else {
				queue->head = node->next;
			}

			if (queue->tail == node) {
				queue->tail = prev;
			}

			queue_node_free(&node, queue->data_free);
			queue->size--;

			if(queue->size == 0){
				queue->head = NULL;
				queue->tail = NULL;
			}

			return true;
		}

		prev = node;
		node = node->next;
	}

	return false;
}

#if DEBUG

void queue_print(queue_t *queue, string_t (*to_string_function)(void *), string_t prefix, bool should_free)
{
	queue_node_t *node = queue->head;

	while (node) {
		string_t data_string = to_string_function(node->data);
		debug_log("%s%s\n", prefix, data_string);

		if(should_free){
			free(data_string);
		}

		node = node->next;
	}
}

#endif // DEBUG

