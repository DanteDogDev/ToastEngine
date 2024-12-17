cmake_minimum_required(VERSION 3.29)
project(Engine)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ensure Threads support is properly configured
set(THREADS_PREFER_PTHREAD_FLAG OFF)
find_package(Threads REQUIRED)

# Define Vulkan SDK Path
set(VULKAN_SDK "C:/VulkanSDK/1.3.296.0")

# Define preprocessor macros
add_compile_definitions(TOASTITO_PLATFORM_WINDOWS TOASTITO_ENGINE_DLL)

# Find all files and add them to the library
file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/Engine/src/*.hpp"
  "${CMAKE_SOURCE_DIR}/Engine/src/*.cpp"
  "${CMAKE_SOURCE_DIR}/Engine/src/*.h"
  "${CMAKE_SOURCE_DIR}/Engine/src/*.c"
)

# Add shared library target for Toast_Engine
add_library(Toast_Engine SHARED
  ${ENGINE_SRC}
  vulkanEngine.hpp
  vulkanEngine.cpp
)

target_precompile_headers(Toast_Engine PRIVATE vulkanEngine.hpp)


# Include directories
target_include_directories(Toast_Engine 
  PUBLIC
    ${VULKAN_SDK}/Include
    ${CMAKE_CURRENT_SOURCE_DIR}
    lib/spdlog/include
)

# Add GLM as a subdirectory
add_subdirectory(lib/glm)

# Add GLFW as a subdirectory and disable unnecessary builds
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw)


# Find Vulkan package
find_package(Vulkan REQUIRED)

# Link libraries to Toast_Engine
target_link_libraries(Toast_Engine
  PUBLIC
    ${CMAKE_THREAD_LIBS_INIT}
    glm::glm
    glfw
    Vulkan::Vulkan
)

# Post-build commands to copy DLLs to specific directories
set(EDITOR_DIR "${CMAKE_BINARY_DIR}/Editor")
set(GAME_DIR "${CMAKE_BINARY_DIR}/Game")

add_custom_command(TARGET Toast_Engine POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory "${EDITOR_DIR}"
  COMMAND ${CMAKE_COMMAND} -E make_directory "${GAME_DIR}"
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:Toast_Engine>" "${EDITOR_DIR}/$<TARGET_FILE_NAME:Toast_Engine>"
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:Toast_Engine>" "${GAME_DIR}/$<TARGET_FILE_NAME:Toast_Engine>"
)
