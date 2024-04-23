//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include "document.h"
#include "../utils/utils.h"

document_t *document_init(string_t name, string_t content)
{
	document_t *document = safe_malloc(sizeof(document_t));

	document->name = create_and_copy(name, (uint (*)(void *))string_data_size);
	document->content = create_and_copy(content, (uint (*)(void *))string_data_size);

	return document;
}

document_t document_copy(document_t document)
{
	document_t copy;

	copy.name = create_and_copy(document.name, (uint (*)(void *))string_data_size);
	copy.content = create_and_copy(document.content, (uint (*)(void *))string_data_size);

	return copy;
}
