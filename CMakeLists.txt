cmake_minimum_required(VERSION 3.8)

project(SwordEngine)

set(CMAKE_C_STANDARD 99)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()

message(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")

# Setup libraries
if (CMAKE_BUILD_TYPE MATCHES Debug)
    include_directories(lib/glad/debug/include)
    add_definitions(-DSD_DEBUG)
    add_library(glad lib/glad/debug/src/glad.c)
elseif (CMAKE_BUILD_TYPE MATCHES Release)
    include_directories(lib/glad/release/include)
    add_library(glad lib/glad/release/src/glad.c)
endif ()

include_directories(lib/stb)

include_directories(include)

# Compile engine
add_library(
    sword
    src/context.c
    src/entity.c
    src/platform.c
    src/render.c
)

set(libs glad)

if (MINGW)
    list(APPEND libs mingw32 SDL2main SDL2)
    add_definitions(-DSD_PLATFORM_WIN32)
elseif (WIN32)
    list(APPEND libs SDL2main SDL2)
    add_definitions(-DSD_PLATFORM_WIN32 -D_CRT_SECURE_NO_WARNINGS)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    list(APPEND libs SDL2)
    add_definitions(-DSD_PLATFORM_MACOS)
endif ()

target_link_libraries(sword ${libs})

add_executable(helloworld example/helloworld.c)
target_link_libraries(helloworld sword)
