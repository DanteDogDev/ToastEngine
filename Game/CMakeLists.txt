cmake_minimum_required(VERSION 3.29)
project(Game)

add_compile_definitions(TOASTITO_PLATFORM_WINDOWS)
set(CMAKE_CXX_STANDARD 23)

add_executable(Toast_Game main.cpp)

target_link_libraries(Toast_Game PUBLIC Toast_Engine)

