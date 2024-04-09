/*
 * Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
 */

#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include "load_balancer.h"

void init(FILE* input);

void process_next_request(FILE* input, load_balancer_t *load_balancer);

void handle_add_server(FILE* input, load_balancer_t *load_balancer);

void handle_remove_server(FILE* input, load_balancer_t *load_balancer);

void handle_edit_document(FILE* input, load_balancer_t *load_balancer);

void handle_get_document(FILE* input, load_balancer_t *load_balancer);

#endif //STATE_MANAGER_H
