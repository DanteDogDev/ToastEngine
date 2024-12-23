#pragma once
// Vulkan and GLFW-related includes
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

// Commonly used headers
#include <cstdint>
#include <deque>
#include <functional>
#include <string>
#include <vector>

// Logging system
#include "src/common/log.hpp"

// Unsigned int types.
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

// Signed int types.
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

// Floating point types.
typedef float f32;
typedef double f64;

// Export macros
#ifdef TOAST_ENGINE_DLL
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif
