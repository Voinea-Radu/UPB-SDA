//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include "../utils/constants.h"

/**
 * @brief The node of a queue
 * @field data 	The data of the node
 * @field next 	The next node in the queue
 */
typedef struct queue_node {
	void *data;
	struct queue_node *next;
} queue_node_t;

/**
 * @brief The queue structure
 * @field head 			The head of the queue
 * @field tail 			The tail of the queue
 * @field size 			The size of the queue
 * @field data_compare 	The function that compares two data
 * @field data_get_size 	The function that gets the size of the data
 * @field data_free 		The function that frees the data
 */
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

/**
 * @brief Initializes a new queue
 * @param data_compare 		The function that compares two data
 * @param data_get_size 	The function that gets the size of the data
 * @param data_free 		The function that frees the data
 * @return A pointer to the newly created queue
 */
queue_t *queue_init(bool (*data_compare)(void *key1, void *key2),
					uint (*data_get_size)(void *data),
					void (*data_free)(void **data));

// ==================== Memory ====================

/**
 * @brief Deallocates the memory used by a queue
 * @param queue 	The queue to deallocate
 */
void queue_free(queue_t **queue);

/**
 * @brief Deallocates the memory used by a queue node
 * @param node 		The node to deallocate
 * @param data_free 	The function that frees the data
 */
void queue_node_free(queue_node_t **node, void (*data_free)(void **data));

// ==================== Functional ====================

/**
 * @brief Adds a new element to the queue
 * @param queue 	The queue to add the element to
 * @param data 		The data to add
 */
void queue_enqueue(queue_t *queue, void *data);

/**
 * @brief Removes the first element from the queue
 * @param queue 	The queue to remove the element from
 * @return The data of the removed element
 */
void *queue_dequeue(queue_t *queue);

/**
 * @brief Checks if the queue is empty
 * @param queue 	The queue to check
 * @return Whether the queue is empty
 */
bool queue_is_empty(queue_t *queue);

/**
 * @brief Removes an element from the queue
 * @param queue 	The queue to remove the element from
 * @param data 		The data to remove
 */
void queue_remove(queue_t *queue, void *data);

#endif  // QUEUE_H
