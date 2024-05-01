//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef DATABASE_H
#define DATABASE_H

#include "../generic/hash_map.h"
#include "dto/document.h"

/**
 * @brief The database structure. This is a key-value store. The key is a string
 * and the value is a string.
 *
 * @param data 	The hash data that stores the key-value pairs. The key is a
 * 				string and the value is a string
 */
typedef struct {
	hash_map_t *data;  // HashMap<String, String>
} database_t;

// ==================== Constructor(s) ====================

/**
 * @brief Initializes a new database
 *
 * @param capacity 	The maximum number of elements the database can store
 * @return The newly created database
 */
database_t *database_init(uint capacity);

// ==================== Memory ====================

/**
 * @brief Deallocates the memory used by a database
 *
 * @param database 	The database to deallocate
 */
void database_free(database_t **database);

// ==================== Functional ====================

/**
 * @brief Puts a new key-value pair in the database
 *
 * @param database 	The database to put the key-value pair in
 * @param key 		The key of the pair
 * @param value 	The value of the pair
 */
void database_put_explicit(database_t *database, string_t key, string_t value);

/**
 * @brief Puts a new key-value pair in the database
 *
 * @param database 	The database to put the key-value pair in
 * @param document 	The document to put in the database
 */
void database_put(database_t *database, document_t *document);

/**
 * @brief Gets the value associated with a key from the database
 *
 * @param database 	The database to get the value from
 * @param key 		The key of the pair
 * @return The value associated with the key
 */
string_t database_get(database_t *database, string_t key);

/**
 * @brief Removes the key-value pair associated with the key from the database
 *
 * @param database 	The database to remove the key-value pair from
 * @param key 		The key of the pair
 */
void database_remove(database_t *database, string_t key);

#endif  // DATABASE_H
