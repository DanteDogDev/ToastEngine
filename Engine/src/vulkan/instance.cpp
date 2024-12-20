#include "instance.hpp"
#include "defines.h"
#include "src/core/log.hpp"
#include <GLFW/glfw3.h>

void ReportVersionNumber(u32 version) {
  ENGINE_DEBUG(                                                                 //
      "System can support Vulkan Variant: {}, Major: {}, Minor: {}, Patch: {}", //
      vk::apiVersionVariant(version),                                           //
      vk::apiVersionMajor(version),                                             //
      vk::apiVersionMinor(version),                                             //
      vk::apiVersionPatch(version)                                              //
  );
}

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
      ENGINE_ERROR("Validation layer '{}' is not supported.", layerName);
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
  ENGINE_DEBUG("extensions to be requested:");
  Engine::Core::Log::PrintList(extensions);

  return extensions;
}

vk::Instance MakeInstance(const char *applicationName, std::deque<std::function<void(vk::Instance)>> &deletionQueue) {
  ENGINE_INFO("Making Instance!");

  if (enableValidationLayers && !checkValidationLayerSupport()) {
    ENGINE_CRITICAL("Validation layers requested, but not available!");
  }

  // Application Info
  u32 version = vk::enumerateInstanceVersion();
  ReportVersionNumber(version);
  vk::ApplicationInfo appInfo;
  appInfo.pApplicationName = applicationName;
  appInfo.applicationVersion = version;
  appInfo.pEngineName = "Vulkat Engine";
  appInfo.engineVersion = version;
  appInfo.apiVersion = version;

  // Extensions
  auto extensions = getRequiredExtensions();
  u32 enabledExtensionCount = static_cast<u32>(extensions.size());
  const char **ppEnabledExtensionNames = extensions.data();
  // Layers
  if (!SupportedByInstance(ppEnabledExtensionNames, enabledExtensionCount, (const char **)validationLayers.data(), validationLayers.size())) {
    ENGINE_CRITICAL("Required instance extensions are not supported!");
  }
  // Instance Creation Info
  vk::InstanceCreateInfo createInfo;
  createInfo.flags = vk::InstanceCreateFlags();
  createInfo.pApplicationInfo = &appInfo;
  createInfo.enabledLayerCount = validationLayers.size();
  createInfo.ppEnabledLayerNames = validationLayers.data();
  createInfo.enabledExtensionCount = enabledExtensionCount;
  createInfo.ppEnabledExtensionNames = ppEnabledExtensionNames;

  vk::Instance instance;
  try {
    instance = vk::createInstance(createInfo);
  } catch (const vk::SystemError &err) {
    ENGINE_CRITICAL("Failed to create Vulkan instance: {}", err.what());
  }

  deletionQueue.push_back([](vk::Instance instance) {
    instance.destroy();
    ENGINE_DEBUG("Deleting Instance");
  });

  return instance;
}
