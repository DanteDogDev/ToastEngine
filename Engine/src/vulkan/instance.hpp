#pragma once
#include <deque>
#include <functional>
#include <vulkan/vulkan.hpp>

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};

/**
 * @brief Create a Vulkan instance.
 *
 * @param applicationName the name of the application.
 * @param deletionQueue Queue onto which to push the instance's destructor.
 *
 * @return the instance created.
 */
vk::Instance MakeInstance(const char *applicationName, std::deque<std::function<void(vk::Instance)>> &deletionQueue);

/**
 * @brief Check whether the requested extensions and layers are supported.
 *
 * @param extensionNames a list of extension names being requested.
 * @param extensionCount the number of requested extensions.
 * @param layerNames a list of layer names being requested.
 * @param layerCount the number of requested layers.
 *
 * @return whether all of the extensions and layers are supported.
 */
bool SupportedByInstance(const char **extensionNames, int extensionCount, const char **layerNames, int layerCount);

/**
 * @brief Logging callback function.
 *
 * @param messageSeverity describes the severity level of the message
 * @param messageType describes the type of the message
 * @param pCallbackData standard data associated with the message
 * @param pUserData custom extra data which can be associated with the message
 *
 * @return whether to end program execution
 */
VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
                                             const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);

/**
 * @brief Make a debug messenger
 *
 * @param instance The Vulkan instance which will be debugged.
 * @param dldi dynamically loads instance based dispatch functions
 * @param deletionQueue stores destructors
 *
 * @return the created messenger
 */
vk::DebugUtilsMessengerEXT MakeDebugMessenger(vk::Instance &instance, vk::DispatchLoaderDynamic &dldi,
                                              std::deque<std::function<void(vk::Instance)>> &deletionQueue);
