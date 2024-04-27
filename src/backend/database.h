//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef DATABASE_H
#define DATABASE_H

#include "../generic/hash_map.h"
#include "dto/document.h"

typedef struct {
	hash_map_t *data;  // HashMap<String, String>
} database_t;

// ==================== Constructor(s) ====================

database_t *database_init(uint capacity);

// ==================== Memory ====================

void database_free(database_t **database);

// ==================== Functional ====================

void database_put_explicit(database_t *database, string_t key, string_t value);

void database_put(database_t *database, document_t *document);

string_t database_get(database_t *database, string_t key);

void database_remove(database_t *database, string_t key);

#endif  // DATABASE_H
