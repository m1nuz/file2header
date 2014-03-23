cmake_minimum_required(VERSION 2.8)

project(file-to-header)

list(APPEND CMAKE_CXX_FLAGS "-Wall")
list(APPEND CMAKE_C_FLAGS "-Wall -std=c99")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

set(headers
    targa.h
    wave.h
    wavefront.h
)

set(sources
    main.c
    targa.c
    wave.c
    wavefront.c
)

add_executable(f2h ${headers} ${sources})
