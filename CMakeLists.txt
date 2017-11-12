cmake_minimum_required(VERSION 3.8)

project(SwordEngine)

set(CMAKE_C_STANDARD 99)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()

message(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")

# Compile engine
add_library(
    sword
    src/sword.c
)

target_link_libraries(sword ${libs})
