//
// Copyright (c) 2024, Voinea Radu-Mihai <contact@voinearadu.com>
//

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <malloc.h>

typedef char *string_t;
typedef unsigned int uint;

#define DEBUG 0

#define TASK_QUEUE_SIZE          1000
#define DATABASE_HASH_TABLE_SIZE 1000
#define MAX_LOG_LENGTH           1000
#define MAX_RESPONSE_LENGTH      4096

#define REQUEST_TYPE_LENGTH     64
#define DOC_NAME_LENGTH         64
#define DOC_CONTENT_LENGTH      4096
#define REQUEST_LENGTH          (REQUEST_TYPE_LENGTH + DOC_NAME_LENGTH \
                                 + DOC_CONTENT_LENGTH)

#define EDIT_REQUEST            "EDIT"
#define GET_REQUEST             "GET"
#define ADD_SERVER_REQUEST      "ADD_SERVER"
#define REMOVE_SERVER_REQUEST   "REMOVE_SERVER"

#define MEMORY_ERROR      "There was an error while allocating memory!\nExiting...\n"

#define LOG_RESPONSE      "[Server %d]-Response: %s\n[Server %d]-Log: %s\n\n"

#define LOG_CACHE_HIT     "Cache HIT for %s"
#define LOG_CACHE_MISS    "Cache MISS for %s"
#define LOG_CACHE_MISS_WITH_EVICT   "Cache MISS for %s - cache entry for %s has been evicted"
#define LOG_FAULT         "Document %s doesn't exist"
#define LOG_LAZY_EXEC     "Task queue size is %d"

#define MSG_A                    "Request- %s %s - has been added to queue"
#define MSG_B                    "Document %s has been overridden"
#define MSG_C                    "Document %s has been created"

#define SERVER_QUEUED            MSG_A
#define DATABASE_ENTRY_EDITED    MSG_B
#define DATABASE_ENTRY_CREATED   MSG_C

typedef enum request_type {
	ADD_SERVER,
	REMOVE_SERVER,

	EDIT_DOCUMENT,
	GET_DOCUMENT
} request_type_t;

#endif  // CONSTANTS_H
