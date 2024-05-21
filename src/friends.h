#ifndef FRIENDS_H
#define FRIENDS_H

#define MAX_COMMAND_LEN 1000
#define MAX_PEOPLE 550
#define MAX_UINT16 65535

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/users.h"
#include "utils/Task1/graph.h"

/**
 * Function that handles the calling of every command from task 1
 *
 * Please add any necessary parameters to the functions
*/
void handle_input_friends(char *input, graph_t *friends_graph);

#endif // FRIENDS_H
