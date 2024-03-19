/*
Nume: Voinea Radu-Mihai
Grupa: 315 CA
*/

#include <stdio.h>
#include "api/state_manager.h"
#include "api/heap.h"

#define MAX_COMMAND_SIZE 100

int main()
{
	string_t command;

	while (1) {
		command = read_line(MAX_COMMAND_SIZE, stdin);

		if (command == NULL) {
			break;
		}

		if (command[0] == '\0') {
			continue;
		}

		to_lower(command);

		int result = process_command(command);
		free(command);

		if (result == EXIT)
			break;
	}
}
