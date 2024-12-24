#include "instance.hpp"
#include "vklog.hpp"
#include "vulkanConfig.hpp"
using namespace Engine::Vulkan;

void InstanceManager::Init() {
  ENGINE_INFO("Creating Instance Object");
  AddRequiredExtensions();
  SupportedByInstance();
  m_instance = MakeInstance("Vulkat Engine");
  m_dldi.init(m_instance, vkGetInstanceProcAddr);
#ifdef DEBUG
  MakeDebugMessenger();
#endif
}

InstanceManager::~InstanceManager() {
  ENGINE_INFO("Deleting Instance Object");
  while (m_instanceDeletionQueue.size() > 0) {
    m_instanceDeletionQueue.back()(m_instance);
    m_instanceDeletionQueue.pop_back();
  }
}

vk::Instance InstanceManager::MakeInstance(const char *appName) {
  ENGINE_INFO("Making Instance!");

  // Application Info
  u32 version = vk::enumerateInstanceVersion();
  LogVersion(version);
  vk::ApplicationInfo appInfo;
  appInfo.pApplicationName = appName;
  appInfo.applicationVersion = version;
  appInfo.pEngineName = "Vulkat Engine";
  appInfo.engineVersion = version;
  appInfo.apiVersion = version;

  // Instance Creation Info
  vk::InstanceCreateInfo createInfo;
  createInfo.flags = vk::InstanceCreateFlags();
  createInfo.pApplicationInfo = &appInfo;
  createInfo.enabledLayerCount = (u32)VulkanConfig::instanceLayers.size();
  createInfo.ppEnabledLayerNames = VulkanConfig::instanceLayers.data();
  createInfo.enabledExtensionCount = (u32)VulkanConfig::instanceExtensions.size();
  createInfo.ppEnabledExtensionNames = VulkanConfig::instanceExtensions.data();
  vk::Instance instance;
  try {
    instance = vk::createInstance(createInfo);
  } catch (const vk::SystemError &err) {
    ENGINE_CRITICAL("Failed to create Vulkan instance: {}", err.what());
  }

  m_instanceDeletionQueue.push_back([](vk::Instance instance) {
    instance.destroy();
    ENGINE_DEBUG("Deleting Instance");
  });
  return instance;
}

void InstanceManager::AddRequiredExtensions() {
  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
  for (u32 i = 0; i < glfwExtensionCount; i++) {
    const char *extension = glfwExtensions[i];
    auto it = std::find(VulkanConfig::instanceExtensions.begin(), VulkanConfig::instanceExtensions.end(), extension);
    if (it == VulkanConfig::instanceExtensions.end()) {
      VulkanConfig::instanceExtensions.push_back(extension);
    }
  }
}

void InstanceManager::SupportedByInstance() {
  ENGINE_DEBUG("Extensions to be requested");
  LogList(VulkanConfig::instanceExtensions);
  std::vector<vk::ExtensionProperties> supportedExtensions = vk::enumerateInstanceExtensionProperties();
  ENGINE_DEBUG("Instance can support these Extensions")
  LogList(supportedExtensions);
  for (const char *request : VulkanConfig::instanceExtensions) {
    bool found = false;
    for (const auto &support : supportedExtensions) {
      if (strcmp(support.extensionName, request) == 0) {
        ENGINE_DEBUG("Extension \"{}\" Supported", request);
        found = true;
        break;
      }
    }
    if (!found) {
      ENGINE_CRITICAL("Extension \"{}\" Not Supported", request);
    }
  }

  ENGINE_DEBUG("Layers to be requested");
  LogList(VulkanConfig::instanceExtensions);
  ENGINE_DEBUG("Instance can support these Layers")
  std::vector<vk::LayerProperties> supportedLayers = vk::enumerateInstanceLayerProperties();
  LogList(supportedLayers);
  for (const char *request : VulkanConfig::instanceLayers) {
    bool found = false;
    for (const auto &support : supportedLayers) {
      if (strcmp(support.layerName, request) == 0) {
        ENGINE_DEBUG("Layer \"{}\" Supported", request);
        found = true;
        break;
      }
    }
    if (!found) {
      ENGINE_CRITICAL("Layer \"{}\" Not Supported", request);
    }
  }
}

vk::SurfaceKHR InstanceManager::MakeSurface(GLFWwindow *window) {
  VkSurfaceKHR rawSurface;
  try {
    glfwCreateWindowSurface(m_instance, window, nullptr, &rawSurface);
  } catch (const std::exception &e) {
    ENGINE_CRITICAL("Failed to create :Surface{}", e.what());
  }
  m_instanceDeletionQueue.push_back([rawSurface](vk::Instance instance) {
    instance.destroySurfaceKHR(rawSurface, nullptr);
    ENGINE_DEBUG("Destroyed Surface");
  });
  return (vk::SurfaceKHR)rawSurface;
}
#ifdef DEBUG                                                   // debug Messenger
VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(                  //
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,    //
    VkDebugUtilsMessageTypeFlagsEXT messageType,               //
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, //
    void *pUserData) {
  (void)messageType;
  (void)pUserData;
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

void InstanceManager::MakeDebugMessenger() {
  ENGINE_INFO("Making Debug Messenger");
  vk::DebugUtilsMessengerCreateInfoEXT createInfo{};
  createInfo.flags = vk::DebugUtilsMessengerCreateFlagBitsEXT();
  createInfo.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                               vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
  createInfo.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
                           vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;
  createInfo.pfnUserCallback = DebugCallback;
  createInfo.pUserData = nullptr;

  vk::DebugUtilsMessengerEXT messenger = m_instance.createDebugUtilsMessengerEXT(createInfo, nullptr, m_dldi);
  VkDebugUtilsMessengerEXT handle = messenger;

  m_instanceDeletionQueue.push_back([handle, this](vk::Instance instance) {
    instance.destroyDebugUtilsMessengerEXT(handle, nullptr, m_dldi);
    ENGINE_DEBUG("Deleted Debug Messenger");
  });
  m_debugMessenger = messenger;
}
#endif
