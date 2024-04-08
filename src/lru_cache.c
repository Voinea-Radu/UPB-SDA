/*
 * Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
 */

#include <stdio.h>
#include <string.h>
#include "api/lru_cache.h"
#include "api/utils.h"

lru_cache *init_lru_cache(unsigned int cache_capacity) {
    /* TODO */
    return NULL;
}

bool lru_cache_is_full(lru_cache *cache) {
    /* TODO */
    return false;
}

void free_lru_cache(lru_cache **cache) {
    /* TODO */
}

bool lru_cache_put(lru_cache *cache, void *key, void *value,
                   void **evicted_key) {
    /* TODO */
    return false;
}

void *lru_cache_get(lru_cache *cache, void *key) {
    /* TODO */
    return NULL;
}

void lru_cache_remove(lru_cache *cache, void *key) {
    /* TODO */
}
