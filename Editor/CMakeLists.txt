cmake_minimum_required(VERSION 3.29)
project(Editor)

add_compile_definitions(TOASTITO_PLATFORM_WINDOWS)
set(CMAKE_CXX_STANDARD 23)

add_executable(Editor main.cpp)

target_link_libraries(Editor PUBLIC Engine)

