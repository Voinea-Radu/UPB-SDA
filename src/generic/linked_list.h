//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>
#include "../utils/constants.h"

/**
 * @brief The node of a linked list
 * @field data 	The data of the node
 * @field next 	The next node in the list
 */
typedef struct node {
	void *data;
	struct node *next;
} node_t;

/**
 * @brief The linked list structure
 * @field head 			The head of the list
 * @field tail 			The tail of the list
 * @field size 			The size of the list
 * @field data_get_size 	The function that gets the size of the data
 * @field data_compare 	The function that compares two data
 * @field data_free 		The function that frees the data
 */
typedef struct {
	node_t *head;
	node_t *tail;

	uint size;

	uint (*data_get_size)(void *value);

	bool (*data_compare)(void *data1, void *data2);

	void (*data_free)(void **data);
} linked_list_t;

// ==================== Constructor(s) ====================

/**
 * @brief Initializes a new linked list
 * @param data_get_size 	The function that gets the size of the data
 * @param data_compare 		The function that compares two data
 * @param data_free 		The function that frees the data
 * @return A pointer to the newly created linked list
 */
linked_list_t *linked_list_init(uint (*data_get_size)(void *value),
								bool (*data_compare)(void *data1, void *data2),
								void (*data_free)(void **data));

// ==================== Memory ====================

/**
 * @brief Deallocates the memory used by a linked list
 * @param list 	The list to deallocate
 */
void linked_list_free(linked_list_t **list);

// ==================== Functional ====================

/**
 * @brief Adds a new element to the linked list
 * @param list 	The list to add the element to
 * @param data 	The data to add
 */
void linked_list_add(linked_list_t *list, void *data);

/**
 * @brief Adds a new element to the linked list at a specific index
 * @param list 	The list to add the element to
 * @param data 	The data to add
 * @param index 	The index to add the element at
 */
void linked_list_add_at_index(linked_list_t *list, void *data, int index);

/**
 * @brief Gets the element at a specific index
 * @param list 	The list to get the element from
 * @param index 	The index of the element
 * @return The element at the index
 */
void *linked_list_get_at_index(linked_list_t *list, int index);

/**
 * @brief Removes an element from the linked list
 * @param list 	The list to remove the element from
 * @param data 	The data to remove
 * @return The removed element
 */
void *linked_list_remove(linked_list_t *list, void *data);

/**
 * @brief Removes an element from the linked list at a specific index
 * @param list 	The list to remove the element from
 * @param index 	The index of the element to remove
 * @return The removed element
 */
void linked_list_print(linked_list_t *list, void (*print_data)(void *data));

#endif  // LINKED_LIST_H
