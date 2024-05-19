//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef DEBUG_H
#define DEBUG_H

#include "utils.h"

#if DEBUG

void debug_init(void);

int debug_log(const char *format, ...);

int debug_log_no_prefix(const char *format, ...);

string_t increase_prefix(string_t prefix);

#endif  // DEBUG

#endif  // DEBUG_H
