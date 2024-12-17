#include "instance.hpp"
#include <iostream>
#include <spdlog/spdlog.h>

bool SupportedByInstance(const char **extensionNames, int extensionCount, const char **layerNames, int layerCount) {
  spdlog::info("Checking instance support for extensions and layers...");

  // Check extension support
  std::vector<vk::ExtensionProperties> supportedExtensions = vk::enumerateInstanceExtensionProperties();

  spdlog::info("Instance can support the following extensions:");
  for (const auto &ext : supportedExtensions) {
    spdlog::info("  {}", ext.extensionName.data()); // Use .data() to convert to const char*
  }

  for (int i = 0; i < extensionCount; ++i) {
    const char *extension = extensionNames[i];
    bool found = false;
    for (const auto &supportedExtension : supportedExtensions) {
      if (strcmp(extension, supportedExtension.extensionName.data()) == 0) { // Compare using .data()
        spdlog::info("Extension \"{}\" is supported!", extension);
        found = true;
        break;
      }
    }
    if (!found) {
      spdlog::error("Extension \"{}\" is not supported!", extension);
      return false;
    }
  }

  // Check layer support
  std::vector<vk::LayerProperties> supportedLayers = vk::enumerateInstanceLayerProperties();

  spdlog::info("Instance can support the following layers:");
  for (const auto &layer : supportedLayers) {
    spdlog::info("  {}", layer.layerName.data()); // Use .data() to convert to const char*
  }

  for (int i = 0; i < layerCount; ++i) {
    const char *layer = layerNames[i];
    bool found = false;
    for (const auto &supportedLayer : supportedLayers) {
      if (strcmp(layer, supportedLayer.layerName.data()) == 0) { // Compare using .data()
        spdlog::info("Layer \"{}\" is supported!", layer);
        found = true;
        break;
      }
    }
    if (!found) {
      spdlog::error("Layer \"{}\" is not supported!", layer);
      return false;
    }
  }

  spdlog::info("All requested extensions and layers are supported!");
  return true;
}
VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
                                             const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) {
  std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

  return VK_FALSE;
}

vk::DebugUtilsMessengerEXT MakeDebugMessenger(vk::Instance &instance, vk::DispatchLoaderDynamic &dldi,
                                              std::deque<std::function<void(vk::Instance)>> &deletionQueue) {
  vk::DebugUtilsMessengerCreateInfoEXT createInfo =
      vk::DebugUtilsMessengerCreateInfoEXT(vk::DebugUtilsMessengerCreateFlagsEXT(),
                                           vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                                               vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
                                           vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
                                               vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
                                           debugCallback, nullptr);

  vk::DebugUtilsMessengerEXT messenger = instance.createDebugUtilsMessengerEXT(createInfo, nullptr, dldi);
  VkDebugUtilsMessengerEXT handle = messenger;
  deletionQueue.push_back([handle, dldi](vk::Instance instance) { instance.destroyDebugUtilsMessengerEXT(handle, nullptr, dldi); });

  return messenger;
}
