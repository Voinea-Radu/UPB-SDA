//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include <stdint-gcc.h>
#include "post.h"
#include "../utils/utils.h"


post_t *post_init(uint32_t id, uint16_t user_id, string_t title)
{
	post_t *post = safe_malloc(sizeof(post_t));

	post->id = id;
	post->user_id = user_id;
	post->title = title;

	post->likes = linked_list_init();
	post->reposts = linked_list_init();

	return post;
}