cmake_minimum_required(VERSION 3.10)
project(tema2)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -g3 -O3")

file(GLOB SOURCES "src/*.c")

add_executable(tema2 ${SOURCES})

target_link_libraries(tema2 m)