//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include "debug.h"
#include "../backend/load_balancer.h"


#if DEBUG

string_t key_to_string(void *key)
{
	return (string_t)key;
}

void cache_print_entry(string_t prefix, string_t key, string_t value)
{
	debug_log("%s- Key: %s, Value: %s\n", prefix, key, value);
}

void cache_print(cache_t *cache, string_t prefix)
{
	string_t new_prefix = increase_prefix(prefix);

	debug_log("%sCache capacity: %d\n", prefix, cache->data->capacity);
	debug_log("%sCache size: %d\n", prefix, cache->data->size);

	debug_log("%sCache entries:\n", prefix);

	hash_map_print(cache->data, new_prefix,
				   (void (*)(string_t, void *, void *))cache_print_entry);

	debug_log("%sCache history:\n", prefix);
	queue_print(cache->history, key_to_string, new_prefix, false);

	free(new_prefix);
}

void load_balancer_print(load_balancer_t *load_balancer)
{
	debug_log("Servers count: %d\n", load_balancer->servers_count);

	node_t *node = load_balancer->servers->head;

	while (node) {
		server_t *server = (server_t *)node->data;
		server_print(server, "");
		node = node->next;
	}
}

string_t queued_task_to_string(request_t *request)
{
	string_t output = safe_malloc(sizeof(char) * 10000);
	sprintf(output, "Request type: %s, Document name: %s, Document content: %s",
			get_request_type_str(request->type),
			request->document->name, request->document->content);
	return output;
}

void server_print(server_t *server, string_t prefix)
{
	string_t __new_prefix = increase_prefix(prefix);
	string_t new_prefix = increase_prefix(__new_prefix);

	debug_log("%sServer with id %u:\n", prefix, server->server_id);
	debug_log("\t%sHash: %u\n", prefix, server->hash);

	debug_log("\t%sDatabase:\n", prefix);
	database_print(server->database, new_prefix);

	debug_log("\t%sCache:\n", prefix);
	cache_print(server->cache, new_prefix);

	debug_log("\t%sTask queue:\n", prefix);
	queue_print(server->task_queue, (string_t (*)(void *))queued_task_to_string,
				new_prefix, true);

	free(__new_prefix);
	free(new_prefix);
}

void database_print_entry(string_t prefix, string_t key, string_t value)
{
	debug_log("%s- Key: %s, Value: %s\n", prefix, key, value);
}

void database_print(database_t *database, string_t prefix)
{
	hash_map_print(database->data, prefix,
				   (void (*)(string_t, void *, void *))database_print_entry);
}

void queue_print(queue_t *queue, string_t (*to_string_function)(void *),
				 string_t prefix, bool should_free)
{
	queue_node_t *node = queue->head;

	while (node) {
		string_t data_string = to_string_function(node->data);
		debug_log("%s%s\n", prefix, data_string);

		if (should_free) {
			free(data_string);
		}

		node = node->next;
	}
}

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

	return new_prefix;
}

#endif  // DEBUG
