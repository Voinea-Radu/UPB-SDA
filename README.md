# Tema 2

## Server

# Server Architecture

### Interaction with the Server

Interaction with the server is done through a request. There are two types of 
requests. To minimize the operations performed by the server, we use a queue, 
whose maximum size is `TASK_QUEUE_SIZE = 1000`, in which we store the requests
that should modify the content of a document. Thus, their execution is postponed
until a `GET_DOCUMENT` type request is received, following which all the 
requests stored in the queue until then will run, and a corresponding
response should be displayed for each request.

### Server Operations

The server defines the following operations:

- Edit a document (`EDIT <document_name> <document_content>`).
- Get the content of a document (`GET <document_name>`).

### Request Execution Flow

#### EDIT

When it needs to be executed, the task is removed from the front of the queue 
and the document content is stored in the cache. If it does not exist, the
content associated with the existing document is brought from the main memory
into the cache; the content associated with the document name is written in the 
cache and in the database.

#### GET

Each operation stored in the task queue is executed (respecting the order 
imposed by the queue) and the response returned by each operation is displayed

After all the tasks in the queue are executed, the server checks if there is an 
entry in the cache:
- If there is, it is returned.
- Otherwise, the server looks in the main memory:
    - If it exists, it is brought into the cache and returned.
    - If it does not exist, an error message is returned.

## Cache - Least Recently Used (LRU) Cache Implementation

The LRU Cache is based on the principle that there is a very high probability
that the most recently accessed data will be accessed again in the near future.
Thus, when the cache is full, the eviction policy specifies that the entry that
was accessed the longest time ago should be removed.

### Implementation

The implementation of such a cache will be done with the following data
structures:

- A queue, which will retain the order in which the cache entries
were accessed. At the end of the list are the most recently interacted 
entries, and at the beginning of the list are the oldest entries.
- A hashmap, where the keys (cache entries) are stored associated with pointers 
to the corresponding nodes in the list.

### Write-Through Policy

When the value associated with a key already existing in the cache needs to 
be updated, we will apply a write-through policy, updating the value both in 
the cache and in the server's local memory database, to maintain coherence 
between the cache and memory.

## Load Balancer

### Server Distribution System

This system allows the distribution of servers to be modified using the 
following commands:

- `ADD_SERVER <server_id> <number_of_cached_documents>`
- `REMOVE_SERVER <server_id>`

Initially, there are no servers in the system. When the load balancer receives
one of the commands, certain documents and tasks are redistributed among the 
servers.

### Consistent Hashing

Consistent Hashing is a distributed hashing method by which when the table is
resized, on average only n / m keys will be remapped, where n is the current 
number of keys, and m is the number of slots. In implementation, an imaginary
circle called a hash ring will be used on which both the names of the documents
and the IDs of the servers are mapped.

### Server Removal

In the event that one of the servers is removed from the system, all its 
replicas are removed from the hash ring, and the documents saved on them are 
remapped to the closest remaining servers (in the direction of the clock).

### Server Addition

In the case of adding a new server to the system, all documents will be taken
from the neighboring servers (successors in the direction of the clock) and 
it will be checked if they will be remapped to the new server or not. If a
document needs to be mapped to the new server, its value will be transferred
from the old server, and the old server will delete it.

## Replication
### Distributed Hash Ring

This project implements a distributed hash ring for ensuring that objects are
distributed as uniformly as possible across servers.

### Mechanism

Each server is added multiple times to the hash ring. These servers are 
referred to as "replicas" or "virtual nodes". This mechanism is achieved by
associating an artificial label with each replica, starting from the base 
server id: `replica_id * 10^5 + server_id;`

