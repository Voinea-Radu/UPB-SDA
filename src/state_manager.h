//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include "backend/load_balancer.h"

/**
 * @brief Initializes the state manager
 * @param input The input file
 */
void init(FILE *input);

/**
 * @brief Processes the next request from the input file
 * @param input The input file
 * @param load_balancer The load balancer
 */
void process_next_request(FILE *input, load_balancer_t *load_balancer);

/**
 * @brief Handles an add server request
 * @param buffer The request buffer
 * @param load_balancer The load balancer
 */
void handle_add_server(string_t buffer, load_balancer_t *load_balancer);

/**
 * @brief Handles a remove server request
 * @param buffer The request buffer
 * @param load_balancer The load balancer
 */
void handle_remove_server(string_t buffer, load_balancer_t *load_balancer);

/**
 * @brief Handles an edit document request
 * @param buffer The request buffer
 * @param load_balancer The load balancer
 */
void handle_edit_document(string_t buffer, load_balancer_t *load_balancer);

/**
 * @brief Handles a get document request
 * @param buffer The request buffer
 * @param load_balancer The load balancer
 */
void handle_get_document(string_t buffer, load_balancer_t *load_balancer);

#endif  // STATE_MANAGER_H
