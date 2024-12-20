#pragma once

/**
 * @brief Checks whether the physical device can support
 * the requested extensions
 *
 * @param device physical device to check.
 * @param ppRequestedExtensions requested extension names
 * @param requestedExtensionCount number of requested extensions
 * @return true if all are supported
 * @return false otherwise
 */
#include <vulkan/vulkan_handles.hpp>
bool Supports(const vk::PhysicalDevice &device, const char **ppRequestedExtensions, const u32 requestedExtensionCount);

/**
 * @brief Checks whether the given device is suitable
 *
 * @param device Physical device to check
 * @return true Physical device is suitable
 * @return false Physical device is unsuitable
 */
bool IsSuitable(const vk::PhysicalDevice &device);

/**
 * @brief Chooses a physical device for use
 *
 * @param instance Vulkan instance which will use the device.
 * @return vk::PhysicalDevice the selected physical device.
 */
vk::PhysicalDevice ChoosePhysicalDevice(const vk::Instance &instance);

/**
 * @brief Query the given physical device for the index of
 *  a queue family.
 *
 * @param physicalDevice Physical device to query.
 * @param queueType Type of queue being queried.
 * @return u32 the index of the queue family,
 *  max upon failure.
 */
u32 FindQueueFamilyIndex(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, vk::QueueFlags queueType);

/**
 * @brief Create a logical device object
 *
 * @param physicalDevice Physical device to be abstracted
 * @param deletionQueue holds deletion functions
 * @return vk::Device An abstraction of the physical device
 */
vk::Device CreateLogicalDevice(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, std::deque<std::function<void(vk::Device)>> &deletionQueue);
