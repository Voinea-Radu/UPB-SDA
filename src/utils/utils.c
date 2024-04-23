//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include "utils.h"

uint hash_uint(uint key)
{
	key = ((key >> 16u) ^ key) * 0x45d9f3b;
	key = ((key >> 16u) ^ key) * 0x45d9f3b;
	key = (key >> 16u) ^ key;

	return key;
}

bool compare_strings(string_t string1, string_t string2)
{
	return strcmp(string1, string2) == 0;
}

uint hash_string(string_t key)
{
	unsigned char *key_string = (unsigned char *)key;
	uint hash = 5381;
	int c;

	while ((c = *(key_string++))){
		hash = ((hash << 5u) + hash) + c;
	}

	return hash;
}

string_t get_request_type_str(request_type_t request_type)
{
	switch (request_type) {
	case ADD_SERVER:
		return ADD_SERVER_REQUEST;
	case REMOVE_SERVER:
		return REMOVE_SERVER_REQUEST;
	case EDIT_DOCUMENT:
		return EDIT_REQUEST;
	case GET_DOCUMENT:
		return GET_REQUEST;
	}

	return NULL;
}

request_type_t get_request_type(string_t request_type_str)
{
	request_type_t type;

	if (!strncmp(request_type_str, ADD_SERVER_REQUEST, strlen(ADD_SERVER_REQUEST)))
		type = ADD_SERVER;
	else if (!strncmp(request_type_str, REMOVE_SERVER_REQUEST, strlen(REMOVE_SERVER_REQUEST)))
		type = REMOVE_SERVER;
	else if (!strncmp(request_type_str, EDIT_REQUEST, strlen(EDIT_REQUEST)))
		type = EDIT_DOCUMENT;
	else if (!strncmp(request_type_str, GET_REQUEST, strlen(GET_REQUEST)))
		type = GET_DOCUMENT;
	else
		exit_with_error("Unknown request type");

	return type;
}

void *safe_malloc(size_t size)
{
#if DEBUG
	void *result = safe_calloc(size);
#else // DEBUG
	void *result = malloc(size);
#endif // DEBUG
	check_or_exit(!result, MEMORY_ERROR);

	return result;
}

void *safe_calloc(size_t size)
{
	void *result = calloc(size, 1);

	check_or_exit(!result, MEMORY_ERROR);

	return result;
}

void *safe_realloc(void *ptr, size_t size)
{
	void *result = realloc(ptr, size);

	check_or_exit(!result, MEMORY_ERROR);

	return result;
}

void exit_with_error(string_t message)
{
	fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__);
	perror(message);
	exit(errno);
}

void check_or_exit(bool condition, string_t message)
{
	if (condition)
		exit_with_error(message);
}

int str_to_int(string_t str)
{
	return strtol(str, NULL, 10); // NOLINT(*-narrowing-conversions)
}

void read_quoted_string(string_t buffer, int buffer_len, int *start, int *end)
{
	*end = -1;

	for (int i = 0; i < buffer_len && buffer[i] != '\0'; ++i) {
		if (buffer[i] != '"')
			continue;

		if (*start == -1) {
			*start = i;
		} else {
			*end = i;
			break;
		}
	}
}

string_t increase_prefix(string_t prefix)
{
	int prefix_len = strlen(prefix);
	string_t new_prefix = safe_malloc(prefix_len + 2);

	strcat(new_prefix, "\t");
	strcat(new_prefix, prefix);

	return new_prefix;
}

#if DEBUG

/**
 * This function exists for the sole reason that CLion debugger on windows igit pus
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

void debug_init(void){
	FILE* file = fopen("debug.log", "w");
	fprintf(file, "");
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
	FILE* file = fopen("debug.log", "a");

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

#endif // DEBUG

string_t server_queued(request_type_t request_type, string_t document_name)
{
	string_t request_type_str = get_request_type_str(request_type);
	string_t output = malloc(strlen(SERVER_QUEUED) + strlen(request_type_str) + strlen(document_name) + 1);
	sprintf(output, SERVER_QUEUED, request_type_str, document_name);
	return output;
}

string_t database_entry_edited(string_t document_name)
{
	string_t output = malloc(strlen(DATABASE_ENTRY_EDITED) + strlen(document_name) + 1);
	sprintf(output, DATABASE_ENTRY_EDITED, document_name);
	return output;
}

string_t database_entry_created(string_t document_name)
{
	string_t output = malloc(strlen(DATABASE_ENTRY_CREATED) + strlen(document_name) + 1);
	sprintf(output, DATABASE_ENTRY_CREATED, document_name);
	return output;
}

string_t log_lazy_exec(uint task_queue_size)
{
	string_t output = malloc(strlen(LOG_LAZY_EXEC) + 10);
	sprintf(output, LOG_LAZY_EXEC, task_queue_size);
	return output;
}

string_t log_cache_hit(string_t document_name)
{
	string_t output = malloc(strlen(LOG_CACHE_HIT) + strlen(document_name) + 1);
	sprintf(output, LOG_CACHE_HIT, document_name);
	return output;

}

string_t log_cache_miss(string_t document_name)
{
	string_t output = malloc(strlen(LOG_CACHE_MISS) + strlen(document_name) + 1);
	sprintf(output, LOG_CACHE_MISS, document_name);
	return output;
}

string_t log_cache_miss_with_evict(string_t document_name, string_t evicted_document_name)
{
	string_t output = malloc(strlen(LOG_CACHE_MISS_WITH_EVICT) + strlen(document_name) + strlen(evicted_document_name) + 1);
	sprintf(output, LOG_CACHE_MISS_WITH_EVICT, document_name, evicted_document_name);
	return output;
}

string_t log_fault(string_t document_name)
{
	string_t output = malloc(strlen(LOG_FAULT) + strlen(document_name) + 1);
	sprintf(output, LOG_FAULT, document_name);
	return output;
}

