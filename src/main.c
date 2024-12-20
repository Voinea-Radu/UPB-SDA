/*
Nume: Voinea Radu-Mihai
Grupa: 315 CA
*/

#include <stdio.h>
#include "api/state_manager.h"
#include "api/heap.h"

#define MAX_COMMAND_SIZE 700

int main(void)
{
	string_t command;

	while (1) {
		command = read_line(MAX_COMMAND_SIZE, stdin);

		if (!command)
			break;

		if (command[0] == '\0')
			continue;

		int result = process_command(command);
		free(command);

		if (result == FINISH)
			break;
	}
}
