#include "src/vulkan/instanceManager.hpp"
#include "GLFW/glfw3.h"
#include <iostream>
#include <stdexcept>

namespace Engine::Renderer {
InstanceManager::~InstanceManager() {
  std::cout << "InstanceManager Deconstruct\n";
  vkDestroyInstance(m_instance, nullptr);
}

void InstanceManager::CreateInstance() {
  std::cout << "Instance Create\n";
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Vulkat Engine";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  createInfo.enabledExtensionCount = glfwExtensionCount;
  createInfo.ppEnabledExtensionNames = glfwExtensions;

  createInfo.enabledLayerCount = 0;

  if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
    throw std::runtime_error("failed to create instance!");
  }
}
} // namespace Engine::Renderer
