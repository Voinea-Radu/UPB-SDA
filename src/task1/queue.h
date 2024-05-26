/*
 * Copyright (c) 2024 Coman Andrei-Madalin <andrei.coman1301@stud.acs.upb.ro>
 */

#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>             // for size_t
#include <stdbool.h>             // for bool
#include "mem_alloc_check.h"     // for DIE
#include "double_linked_list.h"  // for double_linked_list_t

typedef struct queue_t {
	double_linked_list_t *buff;
} queue_t;

queue_t *queue_create(size_t data_size, void *free_data_function);

/*
 * Functia intoarce numarul de elemente din coada al carei pointer este trimis
 * ca parametru.
 */
size_t queue_get_size(queue_t *q);

/*
 * Functia intoarce 1 daca coada este goala si 0 in caz contrar.
 */
bool queue_is_empty(queue_t *q);

/*
 * Functia intoarce primul element din coada, fara sa il elimine.
 */
void *queue_front(queue_t *q);

dll_node_t *queue_dequeue(queue_t *q);

/*
 * Functia introduce un nou element in coada.
 */
void queue_enqueue(queue_t *q, void *new_data);

/*
 * Functia elibereaza toata memoria ocupata de coada.
 */
void queue_free(queue_t *q);

#endif /* QUEUE_H */
