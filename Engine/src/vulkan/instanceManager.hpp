#include <iostream>
#include <vector>
#pragma once
namespace Engine::Renderer {
#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class InstanceManager {
public:
  ~InstanceManager();
  void Init();

private:
  VkInstance m_instance;
  VkDebugUtilsMessengerEXT m_debugMessenger;
  const std::vector<const char *> m_validationLayers = {"VK_LAYER_KHRONOS_validation"};
  /**
   * @brief Creates a Vulkan instance for the engine.
   *
   * This function initializes the Vulkan instance by setting up the necessary
   * application information, checking for the availability of validation layers
   * (if enabled), and querying the required GLFW extensions. It then creates the
   * Vulkan instance using `vkCreateInstance`.
   *
   * @throws std::runtime_error If validation layers are requested but not available,
   * or if the Vulkan instance creation fails.
   */
  void CreateInstance();

  /**
   * @brief Checks if the requested validation layers are supported by the Vulkan implementation.
   *
   * This function enumerates all the available instance layers and checks if each layer in the
   * `m_validationLayers` list is supported. If any of the requested layers are not found,
   * it returns `false`; otherwise, it returns `true`.
   *
   * @return true If all requested validation layers are supported.
   * @return false If any requested validation layer is not supported.
   */
  bool CheckValidationLayerSupport();

  /**
   * @brief Retrieves the required Vulkan extensions for the engine.
   *
   * This function queries GLFW for the extensions required to support Vulkan and adds
   * the `VK_EXT_DEBUG_UTILS_EXTENSION_NAME` extension to the list if validation layers
   * are enabled. It returns a vector of extension names that should be enabled during
   * the Vulkan instance creation.
   *
   * @return A vector of `const char*` representing the required Vulkan extensions.
   */
  std::vector<const char *> GetRequiredExtensions();

  /**
   * @brief Sets up the Vulkan debug messenger for validation layer messages.
   *
   * This function initializes a `VkDebugUtilsMessengerCreateInfoEXT` structure with the desired
   * message severity and types to enable debug callback functionality. It specifies the callback
   * function (`DebugCallback`) that will handle validation layer messages and warnings.
   *
   * and the following message types:
   * - General messages
   * - Validation messages
   * - Performance-related messages.
   */
  void SetupDebugMessenger();

  /**
   * @brief Callback function for Vulkan validation layer debug messages.
   *
   * This function is called by the Vulkan debug messenger when a validation layer
   * message is generated. It outputs the validation layer message to the standard error
   * stream for debugging purposes.
   *
   * @param messageSeverity The severity of the validation message (e.g., error, warning, info).
   * @param messageType The type of the validation message (e.g., general, performance, validation).
   * @param pCallbackData A pointer to a structure containing the details of the message.
   * @param pUserData A pointer to user-defined data passed to the callback function (unused here).
   *
   * @return VK_FALSE to indicate that the message should not be further processed.
   *
   * @note This callback function is used for debugging purposes during the development process.
   * It outputs messages from the Vulkan validation layers to help diagnose issues with the Vulkan API usage.
   */
  static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                      VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                      const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) {

    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
  }
};
} // namespace Engine::Renderer
