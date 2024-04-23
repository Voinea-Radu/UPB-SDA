//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include "database.h"
#include "../utils/utils.h"

database_t *database_init(uint capacity)
{
	database_t *database = (database_t *)malloc(sizeof(database_t));
	database->data = hash_map_init(capacity, (uint (*)(void *))hash_string, (bool (*)(void *, void *))string_equals,
								   (uint (*)(void *))string_data_size, (uint (*)(void *))string_data_size,
								   (void (*)(void **))string_free, (void (*)(void **))string_free);
	return database;
}

void database_free(database_t **database)
{
	hash_map_free(&(*database)->data);
	free(*database);
	*database = NULL;
}

void database_put(database_t *database, document_t document)
{
	hash_map_put(database->data, document.name, document.content);
}

string_t database_get(database_t *database, string_t key)
{
	return hash_map_get(database->data, key);
}

void database_remove(database_t *database, string_t key)
{
	hash_map_remove(database->data, key);
}

#if DEBUG
void database_print_entry(string_t prefix, string_t key, string_t value)
{
	debug_log("%s- Key: %s, Value: %s\n", prefix, key, value);
}

void database_print(database_t *database, string_t prefix)
{
	hash_map_print(database->data, prefix, (void (*)(string_t, void *, void *))database_print_entry);
}
#endif //DEBUG
