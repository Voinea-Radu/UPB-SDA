//
// Copyright (c) 2024, Andrei Otetea <andreiotetea23@gmail.com>
// Copyright (c) 2024, Eduard Marin <marin.eduard.c@gmail.com>
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include <stdio.h>
#include <string.h>

#include "utils/utils.h"
#include "state_manager.h"

int main(int argc, string_t *argv)
{
#if DEBUG
	debug_init();
#endif // DEBUG

	FILE *input;

	if (argc < 2) {
		printf("Usage: %s <input_file>\n", argv[0]);
		return -1;
	}

	input = fopen(argv[1], "rt");
	check_or_exit(input == NULL, "Missing input file");

	init(input);

	fclose(input);

	return 0;
}
