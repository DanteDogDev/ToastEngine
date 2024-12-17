#include "instance.hpp"
#include "defines.h"
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
bool checkValidationLayerSupport() {
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for (const char *layerName : validationLayers) {
    bool layerFound = false;

    for (const auto &layerProperties : availableLayers) {
      if (strcmp(layerName, layerProperties.layerName) == 0) {
        layerFound = true;
        break;
      }
    }

    if (!layerFound) {
      spdlog::error("Validation layer '{}' is not supported.", layerName);
      return false;
    }
  }

  return true;
}
std::vector<const char *> getRequiredExtensions() {
  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

  if (enableValidationLayers) {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  return extensions;
}

vk::Instance MakeInstance(const char *applicationName, std::deque<std::function<void(vk::Instance)>> &deletionQueue) {
  spdlog::info("Making Instance!");

  if (enableValidationLayers && !checkValidationLayerSupport()) {
    throw std::runtime_error("Validation layers requested, but not available!");
  }

  u32 version = vk::enumerateInstanceVersion();

  vk::ApplicationInfo appInfo = vk::ApplicationInfo( //
      applicationName,                               //
      version,                                       //
      nullptr,                                       //
      version,                                       //
      version                                        //
  );

  // Extensions
  auto extensions = getRequiredExtensions();
  u32 enabledExtensionCount = static_cast<u32>(extensions.size());
  const char **ppEnabledExtensionNames = extensions.data();

  if (!SupportedByInstance(ppEnabledExtensionNames, enabledExtensionCount, ppEnabledExtensionNames, enabledExtensionCount)) {
    return nullptr;
  }
  vk::InstanceCreateInfo createInfo = vk::InstanceCreateInfo( //
      vk::InstanceCreateFlags(),                              //
      &appInfo,                                               //
      0, nullptr,                                             //
      enabledExtensionCount, ppEnabledExtensionNames          //
  );
  if (enableValidationLayers) {
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
  } else {
    createInfo.enabledLayerCount = 0;
  }
  vk::Instance instance;
  try {
    instance = vk::createInstance(createInfo);
  } catch (const vk::SystemError &err) {
    spdlog::error("Failed to create Vulkan instance: {}", err.what());
    throw;
  }

  deletionQueue.push_back([](vk::Instance instance) {
    instance.destroy();
    spdlog::info("Deleting Instance");
  });

  return instance;
}
