//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include "backend/load_balancer.h"

void init(FILE *input);

void process_next_request(FILE *input, load_balancer_t *load_balancer);

void handle_add_server(string_t buffer, load_balancer_t *load_balancer);

void handle_remove_server(string_t buffer, load_balancer_t *load_balancer);

void handle_edit_document(string_t buffer, load_balancer_t *load_balancer);

void handle_get_document(string_t buffer, load_balancer_t *load_balancer);

#endif  // STATE_MANAGER_H
