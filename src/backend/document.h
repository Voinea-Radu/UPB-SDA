//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "../utils/constants.h"

typedef struct {
	string_t name;
	string_t content;
} document_t;

#endif //DOCUMENT_H

document_t *document_init(string_t name, string_t content);
