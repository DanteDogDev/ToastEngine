#include "instance.hpp"
#include "vklog.hpp"

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
  LogList(extensions);

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
