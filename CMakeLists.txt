cmake_minimum_required(VERSION 3.29)
project(Toast)

# Enforce 64-bit architecture for GCC/Clang compilers
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m64")
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin/int)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(Engine)
add_subdirectory(Editor)
add_subdirectory(Game)
