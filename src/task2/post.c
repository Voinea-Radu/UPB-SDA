//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include <stdint-gcc.h>
#include "post.h"
#include "../utils/utils.h"


void free_post(void *data)
{
	data = data;
	// TODO
}

bool compare_post(void *data1, void *data2)
{
	return ((post_t *)data1)->id == ((post_t *)data2)->id;
}

post_t *post_init(uint32_t id, uint16_t user_id, string_t title)
{
	post_t *post = safe_malloc(sizeof(post_t));

	post->id = id;
	post->user_id = user_id;
	post->title = title;

	post->likes = linked_list_init(free_int, compare_int);
	post->reposts = linked_list_init(free_post, compare_post);

	return post;
}