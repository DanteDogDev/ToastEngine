#include "instance.hpp"
#include "src/core/log.hpp"
#include <iostream>

void LogExtensions(std::vector<vk::ExtensionProperties> &extensions) {
  for (const vk::ExtensionProperties &extension : extensions) {
    ENGINE_DEBUG("\t\'{}\'", std::string(extension.extensionName.data()));
  }
}

void LogLayers(std::vector<vk::LayerProperties> &layers) {
  for (const vk::LayerProperties &layer : layers) {
    ENGINE_DEBUG("\t\'{}\'", std::string(layer.layerName.data()));
  }
}
bool SupportedByInstance(const char **extensionNames, int extensionCount, const char **layerNames, int layerCount) {

  std::stringstream lineBuilder;

  // check extension support
  std::vector<vk::ExtensionProperties> supportedExtensions = vk::enumerateInstanceExtensionProperties();

  ENGINE_DEBUG("Instance can support the following extensions:");
  LogExtensions(supportedExtensions);

  bool found;
  for (int i = 0; i < extensionCount; ++i) {
    const char *extension = extensionNames[i];
    found = false;
    for (vk::ExtensionProperties supportedExtension : supportedExtensions) {
      if (strcmp(extension, supportedExtension.extensionName) == 0) {
        found = true;
        lineBuilder << "Extension \"" << extension << "\" is supported!";
        ENGINE_DEBUG(lineBuilder.str());
        lineBuilder.str("");
        break;
      }
    }
    if (!found) {
      lineBuilder << "Extension \"" << extension << "\" is not supported!";
      ENGINE_DEBUG(lineBuilder.str());
      return false;
    }
  }

  // check layer support
  std::vector<vk::LayerProperties> supportedLayers = vk::enumerateInstanceLayerProperties();

  ENGINE_DEBUG("Instance can support the following layers:");
  LogLayers(supportedLayers);

  for (int i = 0; i < layerCount; ++i) {
    const char *layer = layerNames[i];
    found = false;
    for (vk::LayerProperties supportedLayer : supportedLayers) {
      if (strcmp(layer, supportedLayer.layerName) == 0) {
        found = true;
        lineBuilder << "Layer \"" << layer << "\" is supported!";
        ENGINE_DEBUG(lineBuilder.str());
        lineBuilder.str("");
        break;
      }
    }
    if (!found) {
      lineBuilder << "Layer \"" << layer << "\" is not supported!";
      ENGINE_DEBUG(lineBuilder.str());
      lineBuilder.str("");
      // return false;
    }
  }

  return true;
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(                  //
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,    //
    VkDebugUtilsMessageTypeFlagsEXT messageType,               //
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, //
    void *pUserData) {
  if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
    ENGINE_ERROR("Validation layer: {}", pCallbackData->pMessage);
  } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
    ENGINE_WARN("Validation layer: {}", pCallbackData->pMessage);
  } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
    ENGINE_INFO("Validation layer: {}", pCallbackData->pMessage);
  } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
    ENGINE_DEBUG("Validation layer: {}", pCallbackData->pMessage);
  }

  return VK_FALSE;
}

vk::DebugUtilsMessengerEXT MakeDebugMessenger(vk::Instance &instance, vk::DispatchLoaderDynamic &dldi,
                                              std::deque<std::function<void(vk::Instance)>> &deletionQueue) {
  vk::DebugUtilsMessengerCreateInfoEXT createInfo{};
  createInfo.flags = vk::DebugUtilsMessengerCreateFlagBitsEXT();
  createInfo.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                               vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
  createInfo.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
                           vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;
  createInfo.pfnUserCallback = debugCallback;
  createInfo.pUserData = nullptr;

  vk::DebugUtilsMessengerEXT messenger = instance.createDebugUtilsMessengerEXT(createInfo, nullptr, dldi);
  VkDebugUtilsMessengerEXT handle = messenger;
  deletionQueue.push_back([handle, dldi](vk::Instance instance) {
    instance.destroyDebugUtilsMessengerEXT(handle, nullptr, dldi);
    ENGINE_DEBUG("Deleted Debug Messenger");
  });

  return messenger;
}
