/*
 * Copyright (c) 2024 Coman Andrei-Madalin <andrei.coman1301@stud.acs.upb.ro>
 */

#include "double_linked_list.h"

dll_node_t *dll_node_alloc(void) {
	dll_node_t *new_node = malloc(sizeof(*new_node));
	DIE(!new_node, "malloc() failed");

	new_node->prev = NULL;
	new_node->next = NULL;
	new_node->data = NULL;

	return new_node;
}

double_linked_list_t *dll_alloc(void) {
	double_linked_list_t *new_list = calloc(1, sizeof(*new_list));
	DIE(!new_list, "calloc() failed");

	new_list->head = NULL;
	new_list->tail = NULL;

	return new_list;
}

double_linked_list_t *
dll_list_init(size_t data_size, void *free_data_function) {
	double_linked_list_t *new_list = dll_alloc();

	new_list->free_data_function = free_data_function;
	new_list->data_size = data_size;
	new_list->size = 0;

	return new_list;
}

size_t dll_get_size(double_linked_list_t *list) {
	return list->size;
}

bool dll_is_empty(double_linked_list_t *list) {
	return dll_get_size(list) == 0;
}

void dll_set_head(double_linked_list_t *list, dll_node_t *node) {
	if (!list->head) {
		list->head = node;
		list->tail = node;
		node->next = NULL;
		node->prev = NULL;
	} else {
		node->next = list->head;
		list->head->prev = node;
		list->head = node;
		node->prev = NULL;
	}

	list->size++;
}

void dll_add_nth_node(double_linked_list_t *list, size_t n, const void *data) {
	dll_node_t *new_node = dll_node_alloc();

	new_node->data = malloc(list->data_size);
	DIE(!new_node->data, "malloc() failed");

	memcpy(new_node->data, data, list->data_size);

	if (n == 0) {
		dll_set_head(list, new_node);
		return;
	}

	dll_node_t *curr_node = dll_get_head(list);
	for (size_t i = 0; i < n - 1; i++) {
		if (!curr_node) {
			printf("Index out of bounds!\n");
			exit(1);
		}
		curr_node = curr_node->next;
	}

	new_node->next = curr_node->next;
	new_node->prev = curr_node;
	curr_node->next = new_node;

	if (new_node->next)
		new_node->next->prev = new_node;
	else
		list->tail = new_node;

	list->size++;
}

dll_node_t *dll_remove_nth_node(double_linked_list_t *list, size_t n) {
	if (n == 0)
		return dll_remove_head(list);

	dll_node_t *curr_node = dll_get_head(list);
	for (size_t i = 0; i < n; i++) {
		if (!curr_node) {
			printf("Index out of bounds!\n");
			exit(1);
		}
		curr_node = curr_node->next;
	}

	curr_node->prev->next = curr_node->next;
	if (curr_node->next)
		curr_node->next->prev = curr_node->prev;
	else
		list->tail = curr_node->prev;

	list->size--;

	return curr_node;
}

dll_node_t *dll_get_head(double_linked_list_t *list) {
	return list->head;
}

dll_node_t *dll_get_tail(double_linked_list_t *list) {
	return list->tail;
}

dll_node_t *dll_remove_head(double_linked_list_t *list) {
	dll_node_t *to_remove = list->head;

	if (!to_remove) {
		printf("List is empty!\n");
		exit(1);
	}

	list->head = to_remove->next;
	list->size--;

	to_remove->next = NULL;
	to_remove->prev = NULL;

	if (!list->head)
		list->tail = NULL;
	else
		list->head->prev = NULL;

	return to_remove;
}

void dll_add_tail(double_linked_list_t *list, dll_node_t *node) {
	if (!list->tail) {
		list->head = node;
		list->tail = node;
		node->prev = NULL;
		node->next = NULL;
		list->size++;
		return;
	}

	list->tail->next = node;
	node->prev = list->tail;
	node->next = NULL;
	list->tail = node;
	list->size++;
}

void dll_add_tail_value(double_linked_list_t *list, void *value){
	dll_node_t *new_node = dll_node_alloc();

	new_node->data = malloc(list->data_size);
	DIE(!new_node->data, "malloc() failed");

	memcpy(new_node->data, value, list->data_size);

	dll_add_tail(list, new_node);

}

void dll_refresh_node(double_linked_list_t *list, dll_node_t *node) {
	if (node == list->tail)
		return;

	if (node == list->head) {
		list->head = node->next;
		list->head->prev = NULL;
	} else {
		node->prev->next = node->next;
		node->next->prev = node->prev;
	}

	list->tail->next = node;
	node->prev = list->tail;
	node->next = NULL;
	list->tail = node;
}

void dll_list_free(double_linked_list_t *list) {
	dll_node_t *curr_node = dll_get_head(list);
	while (curr_node) {
		dll_node_t *to_del = curr_node;
		curr_node = curr_node->next;

		list->free_data_function(to_del->data);

		free(to_del);
		to_del = NULL;
	}

	free(list);
}

dll_node_t *dll_remove_tail(double_linked_list_t *list) {
	dll_node_t *to_remove = list->tail;

	if (!to_remove) {
		printf("List is empty!\n");
		exit(1);
	}

	list->tail = to_remove->prev;
	list->size--;

	if (!list->tail)
		list->head = NULL;
	else
		list->tail->next = NULL;

	return to_remove;
}

/**
 *
 * @param list: A double linked list
 * @param value: Value you want to add to list
 * @param cmp: Compare function, needs to return 1 if the data already in the
 * list is "bigger", 0 if they are "equal, -1 otherwise.
 * @return Return the node added
 */
dll_node_t *dll_list_add_sorted(double_linked_list_t *list, void *value,
								int (*cmp)(void *, void *)) {
	dll_node_t *new_node = dll_node_alloc();

	new_node->data = malloc(list->data_size);

	memcpy(new_node->data, value, list->data_size);

	dll_node_t *curr_node = dll_get_head(list);
	dll_node_t *prev = NULL;
	while (curr_node) {
		int cmp_val = cmp(curr_node->data, new_node->data);

		if(cmp_val == 0){
			free(new_node->data);
			free(new_node);
			return NULL;
		}

		if (cmp_val > 0) {
			prev = curr_node;
			curr_node = curr_node->next;
			continue;
		}

		break;
	}

	list->size++;

	if (!prev && !curr_node) {
		list->head = new_node;
		list->tail = new_node;
		return new_node;
	}

	if (!prev) {
		new_node->next = curr_node;

		if (curr_node)
			curr_node->prev = new_node;

		list->head = new_node;
		return new_node;
	}

	if (!curr_node) {
		new_node->prev = prev;

		prev->next = new_node;

		list->tail = new_node;
		return new_node;
	}

	prev->next = new_node;
	new_node->prev = prev;

	curr_node->prev = new_node;
	new_node->next = curr_node;

	return new_node;
}

dll_node_t *
dll_list_add_sorted_with_limit(double_linked_list_t *list, void *value,
							   int (*cmp)(void *, void *), size_t limit) {
	dll_node_t *new_node = dll_node_alloc();
	new_node->data = value;
//	In this situation, we do not need to copy the data
//	memcpy(new_node->data, value, list->data_size);

	dll_node_t *curr_node = dll_get_head(list);
	dll_node_t *prev = NULL;
	while (curr_node) {
		int cmp_val = cmp(curr_node->data, new_node->data);

		if (cmp_val > 0) {
			prev = curr_node;
			curr_node = curr_node->next;
			continue;
		}

		break;
	}

	list->size++;

	if (!prev && !curr_node) {
		list->head = new_node;
		list->tail = new_node;
		return new_node;
	}

	if (!prev) {
		new_node->next = curr_node;

		if (curr_node)
			curr_node->prev = new_node;

		list->head = new_node;

		if (list->size > limit) {
			dll_node_t *to_remove = dll_remove_tail(list);
			list->free_data_function(to_remove->data);
			free(to_remove);
		}

		return new_node;
	}

	if (!curr_node) {
		if (list->size > limit) {
			list->free_data_function(new_node->data);
			free(new_node);
			return NULL;
		}
		new_node->prev = prev;

		prev->next = new_node;

		list->tail = new_node;
		return new_node;
	}

	prev->next = new_node;
	new_node->prev = prev;

	curr_node->prev = new_node;
	new_node->next = curr_node;

	if (list->size > limit) {
		dll_node_t *to_remove = dll_remove_tail(list);
		list->free_data_function(to_remove->data);
		free(to_remove);
	}

	return new_node;
}

/**
 *
 * @param list: A double linked list
 * @param value: Value you want to add to list
 * @param cmp: Compare function, needs to return 1 for the data you want to
 * remove
 * @return Return the node removed, NULL if no node was removed
 */
dll_node_t *dll_list_remove_custom(double_linked_list_t *list, void *value,
								   int (*cmp)(void *, void *)) {
	dll_node_t *curr_node = dll_get_head(list);
	dll_node_t *prev = NULL;

	while (curr_node) {
		int cmp_val = cmp(curr_node->data, value);

		if (cmp_val != 0) {
			prev = curr_node;
			curr_node = curr_node->next;
			continue;
		}

		if (prev)
			prev->next = curr_node->next;
		else
			list->head = curr_node->next;

		if (curr_node->next)
			curr_node->next->prev = prev;
		else
			list->tail = prev;

		list->size--;
		return curr_node;
	}

	return NULL;
}

void dll_add_head(double_linked_list_t *list, void *value) {
	dll_node_t *new_node = dll_node_alloc();

	new_node->data = malloc(list->data_size);
	DIE(!new_node->data, "malloc() failed");

	memcpy(new_node->data, value, list->data_size);

	if (!list->head) {
		list->head = new_node;
		list->tail = new_node;
		new_node->next = NULL;
		new_node->prev = NULL;
	} else {
		new_node->next = list->head;
		list->head->prev = new_node;
		list->head = new_node;
		new_node->prev = NULL;
	}

	list->size++;
}
