//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include "database.h"
#include "../utils/utils.h"
#include "../generic/hash_map.h"

database_t *database_init(unsigned int capacity)
{
	database_t *database = (database_t *)malloc(sizeof(database_t));
	database->data = hash_map_init(capacity, (uint (*)(void *))hash_string);
	return database;
}

void database_free(database_t **database)
{
	hash_map_free(&(*database)->data);
	free(*database);
	*database = NULL;
}

void database_put(database_t *database, string_t key, string_t value)
{
	hash_map_put(database->data, key, value);
}

string_t database_get(database_t *database, string_t key)
{
	return hash_map_get(database->data, key);
}

void database_remove(database_t *database, string_t key)
{
	hash_map_remove(database->data, key);
}

void database_print(database_t *database, string_t prefix)
{
	hash_map_print(database->data, prefix);
}
