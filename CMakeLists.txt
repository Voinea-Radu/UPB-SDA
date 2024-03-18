cmake_minimum_required(VERSION 3.22.1)
project(tema1 C)

set(CMAKE_C_STANDARD 11)

add_executable(tema1
        src/api/state_manager.h
        src/api/string_utils.h
        src/api/linked_list.h
        src/api/utils.h
        src/api/heap.h
        src/state_manager.c
        src/string_utils.c
        src/linked_list.c
        src/utils.c
        src/heap.c
        src/main.c

)
