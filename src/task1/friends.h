/*
 * Copyright (c) 2024 Coman Andrei-Madalin <andrei.coman1301@stud.acs.upb.ro>
 */

#ifndef FRIENDS_H
#define FRIENDS_H

#define MAX_COMMAND_LEN 1000
#define MAX_PEOPLE 550
#define MAX_UINT16 65535

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/users.h"
#include "../generic/graph.h"

/**
 * Function that handles the calling of every command from task 1
 *
 * Please add any necessary parameters to the functions
*/
void handle_input_friends(const char *input, graph_t *friends_graph);

double_linked_list_t *get_friends(graph_t *friends, uint16_t id);

graph_t *get_all_friends();

#endif // FRIENDS_H
