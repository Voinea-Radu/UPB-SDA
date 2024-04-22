//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef DATABASE_H
#define DATABASE_H

#include "../generic/hash_map.h"
#include "document.h"

typedef struct {
	hash_map_t *data;
} database_t;

database_t* database_init(uint capacity);

void database_free(database_t **database);

void database_put(database_t *database, document_t document);

string_t database_get(database_t *database, string_t key);

void database_remove(database_t *database, string_t key);

void database_print(database_t *database, string_t prefix);

#endif //DATABASE_H
