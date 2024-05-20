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
	uint16_t user_id;
	string_t title;

	linked_list_t *likes; // list<int> / list<user_id>
	linked_list_t *reposts; // list<post_t>
} post_t;

post_t *post_init(uint32_t id, uint16_t user_id, string_t title);

void free_post(void *data);

bool compare_post(void *data1, void *data2);

#endif // POST_H
