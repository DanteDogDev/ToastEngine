cmake_minimum_required(VERSION 3.29)
project(Toastito_Engine)

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


# Add shared library target for Engine
add_library(Engine SHARED
  engine.hpp
  engine.cpp
)

target_precompile_headers(Engine PRIVATE engine.hpp)


# Include directories
target_include_directories(Engine 
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

# Link libraries to Engine
target_link_libraries(Engine
  PUBLIC
    ${CMAKE_THREAD_LIBS_INIT}
    glm::glm
    glfw
    Vulkan::Vulkan
)

# Post-build commands to copy DLLs to specific directories
set(EDITOR_DIR "${CMAKE_BINARY_DIR}/Editor")
set(GAME_DIR "${CMAKE_BINARY_DIR}/Game")

add_custom_command(TARGET Engine POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory "${EDITOR_DIR}"
  COMMAND ${CMAKE_COMMAND} -E make_directory "${GAME_DIR}"
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:Engine>" "${EDITOR_DIR}/$<TARGET_FILE_NAME:Engine>"
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:Engine>" "${GAME_DIR}/$<TARGET_FILE_NAME:Engine>"
)
