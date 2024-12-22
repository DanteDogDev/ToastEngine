cmake_minimum_required(VERSION 3.29)
project(Editor)

add_compile_definitions(TOASTITO_PLATFORM_WINDOWS)
set(CMAKE_CXX_STANDARD 23)

add_executable(Toast_Editor main.cpp)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_definitions(Toast_Editor PRIVATE DEBUG)
endif()


target_link_libraries(Toast_Editor PUBLIC Toast_Engine)

