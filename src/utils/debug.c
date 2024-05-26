//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include <string.h>
#include "debug.h"
#include "users.h"

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

string_t increase_prefix(string_t prefix)
{
	int prefix_len = strlen(prefix);
	string_t new_prefix = safe_malloc(prefix_len + 2);

	strcat(new_prefix, "\t");
	strcat(new_prefix, prefix);
	new_prefix[prefix_len + 1] = '\0';

	return new_prefix;
}

void print_username(string_t prefix, uint16_t *user_id)
{
	uint16_t id = *user_id;
	id = id;
	debug_log("%s%s\n", prefix, get_username(*user_id));
}

void print_post(string_t prefix, post_t *post)
{
	if (strlen(prefix) == 0) {
		debug_log("Post %d: %s by %s\n", post->id, post->title,
				  get_username(post->user_id));
	} else {
		debug_log("%sReposed by %s\n", prefix, get_username(post->user_id));
	}

	debug_log("%sLikes: %d\n", prefix, post->likes->size);

	prefix = increase_prefix(prefix);
	linked_list_print_prefixed(post->likes, prefix,
							   (void (*)(string_t, void *))print_username);

	if (post->reposts->size != 0) {
		debug_log("%sReposts:\n", prefix);
		linked_list_print_prefixed(post->reposts, prefix,
								   (void (*)(string_t, void *))print_post);
	}
}

#endif  // DEBUG
