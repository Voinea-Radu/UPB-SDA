//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include "document.h"
#include "../utils/utils.h"

document_t *document_init(string_t name, string_t content)
{
	document_t *document = safe_malloc(sizeof(document_t));

	document->name = name;
	document->content = content;

	return document;
}
