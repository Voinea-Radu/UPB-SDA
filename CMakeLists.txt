cmake_minimum_required(VERSION 3.27)
project(tema2 C)

set(CMAKE_C_STANDARD 11)

add_executable(tema2
        src/load_balancer.c
        src/cache.c
        src/main.c
        src/replication.c
        src/server.c
        src/utils.c
        src/state_manager.c
        src/queue.c
        src/hash_map.c

        src/api/constants.h
        src/api/load_balancer.h
        src/api/cache.h
        src/api/replication.h
        src/api/server.h
        src/api/utils.h
        src/api/state_manager.h
        src/api/queue.h
        src/api/hash_map.h
)
