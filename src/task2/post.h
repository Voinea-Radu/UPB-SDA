//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef POST_H
#define POST_H

#include <stdint-gcc.h>
#include "../utils/constants.h"
#include "../generic/linked_list.h"

typedef struct {
	uint32_t id;
	uint32_t user_id;
	string_t title;

	linked_list_t *likes; // list<int> / list<user_id>
	linked_list_t *reposts; // list<post_t>
} post_t;

post_t *post_init(uint32_t id, uint32_t user_id, string_t title);

#endif // POST_H
