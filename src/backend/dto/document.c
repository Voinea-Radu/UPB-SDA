//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#include "document.h"
#include "../../utils/utils.h"

document_t *document_init(string_t name, string_t content)
{
	document_t *document = safe_malloc(sizeof(document_t));

	document->name = create_and_copy(name, (uint (*)(void *))string_data_size);
	document->content = create_and_copy(content,
										(uint (*)(void *))string_data_size);

	return document;
}

void document_free(document_t **document)
{
	if (document == NULL || *document == NULL)
		return;

	string_free(&(*document)->name);
	string_free(&(*document)->content);

	free(*document);
	*document = NULL;
}

uint document_hash(document_t *document)
{
	return string_hash(document->name);
}

