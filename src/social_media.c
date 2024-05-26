//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils/users.h"
#include "task1/friends.h"
#include "task2/task2.h"
#include "task3/feed.h"
#include "utils/debug.h"

// Private methods
void __init_tasks(void);

void __free_tasks(void);

void __init_tasks(void)
{
#ifdef TASK_1

#endif

#ifdef TASK_2

#endif

#ifdef TASK_3

#endif
}

void __free_tasks(void)
{
#ifdef TASK_1

#endif

#ifdef TASK_2
	posts_free_memory(get_all_posts());
#endif

#ifdef TASK_3

#endif
}

int main(void)
{
#if DEBUG
	debug_init();
#endif
	init_users();
	__init_tasks();

	char *input = malloc(MAX_COMMAND_LEN);

	while (1) {
		char *result = fgets(input, MAX_COMMAND_LEN, stdin);

		// If fgets returns NULL, we reached EOF
		if (!result)
			break;

#ifdef TASK_1
		handle_input_friends(input, get_all_friends());
#endif

#ifdef TASK_2
		handle_input_posts(input);
#endif

#ifdef TASK_3
		handle_input_feed(input);
#endif
	}

	free(input);
	__free_tasks();
	graph_free(get_all_friends());
	free_users();

	return 0;
}
