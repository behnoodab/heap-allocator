cmake_minimum_required(VERSION 3.16)
project(heap_allocator C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(include)

add_library(heap_alloc
    src/heap_alloc.c
)

add_executable(test_allocator
    test/main.c
)

target_link_libraries(test_allocator heap_alloc)
