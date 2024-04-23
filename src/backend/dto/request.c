//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include <stdbool.h>
#include "request.h"
#include "../../utils/utils.h"

request_t *request_init(request_type_t type, document_t *document)
{
	request_t *request = safe_malloc(sizeof(request_t));
	request->type = type;
	request->document = document;
	return request;
}

void request_free(request_t **request)
{
	document_free(&(*request)->document);

	free(*request);
	*request = NULL;
}

bool request_equal(request_t *request1, request_t *request2)
{
	return request1->type == request2->type && strcmp(request1->document->name, request2->document->name) == 0;
}

uint request_size(__attribute__((unused)) request_t *request)
{
	return sizeof(request_t);
}

