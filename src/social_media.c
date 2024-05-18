//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils/users.h"
#include "friends.h"
#include "task2/task2.h"
#include "feed.h"
#include "utils/debug.h"

void init_tasks(void)
{
	#ifdef TASK_1

	#endif

	#ifdef TASK_2

	#endif

	#ifdef TASK_3

	#endif
}

int main(void)
{
	debug_init();
	init_users();
	init_tasks();

	char *input = (char *)malloc(MAX_COMMAND_LEN);
	while (1) {
		input = fgets(input, MAX_COMMAND_LEN, stdin);

		// If fgets returns null, we reached EOF
		if (!input)
			break;

		#ifdef TASK_1
		handle_input_friends(input);
		#endif

		#ifdef TASK_2
		handle_input_posts(input);
		#endif

		#ifdef TASK_3
		handle_input_feed(input);
		#endif
	}

	return 0;
}
