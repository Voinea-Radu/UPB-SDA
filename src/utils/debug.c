//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include "debug.h"

#if DEBUG

/**
 * This function exists for the sole reason that CLion debugger on windows
 * not printing any output unless fflush is called
 */
int printf(const char *format, ...)
{
	__builtin_va_list argv;
	__builtin_va_start(argv, format);

	string_t output = safe_malloc(10000);

	vsprintf(output, format, argv);
	fprintf(stdout, "%s", output);

	__builtin_va_end(argv);
	fflush(stdout);

	debug_log_no_prefix("%s", output);

	free(output);

	return 0;
}

void debug_init(void)
{
	FILE *file = fopen("debug.log", "w");
	fclose(file);
}

int debug_log(const char *format, ...)
{
	__builtin_va_list argv;
	__builtin_va_start(argv, format);

	string_t output_1 = safe_malloc(10000);
	string_t output_2 = safe_malloc(10000);

	vsprintf(output_1, format, argv);
	sprintf(output_2, "[DEBUG] %s", output_1);

	debug_log_no_prefix("%s", output_2);

	free(output_1);
	free(output_2);

	__builtin_va_end(argv);
	fflush(stdout);

	return 0;
}

int debug_log_no_prefix(const char *format, ...)
{
	FILE *file = fopen("debug.log", "a");

	__builtin_va_list argv;
	__builtin_va_start(argv, format);

	string_t output = safe_malloc(10000);

	vsprintf(output, format, argv);

	fprintf(file, "%s", output);

	free(output);

	__builtin_va_end(argv);
	fflush(stdout);
	fclose(file);

	return 0;
}

#endif  // DEBUG
