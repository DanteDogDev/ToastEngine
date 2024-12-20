#include "renderer.hpp"
#include "GLFW/glfw3.h"
#include "device.hpp"
#include "instance.hpp"

namespace Engine::Vulkan {
Renderer::Renderer(GLFWwindow *window) {
  ENGINE_INFO("Making Renderer");
  m_instance = MakeInstance("Vulkat Engine", m_instanceDeletionQueue);
  // m_dldi = vk::DispatchLoaderDynamic(m_instance, vkGetInstanceProcAddr);
  m_dldi.init(m_instance, vkGetInstanceProcAddr);
  m_debugMessenger = MakeDebugMessenger(m_instance, m_dldi, m_instanceDeletionQueue);

  VkSurfaceKHR rawSurface;
  glfwCreateWindowSurface(m_instance, window, nullptr, &rawSurface);
  m_surface = rawSurface;
  m_instanceDeletionQueue.push_back([this](vk::Instance instance) {
    instance.destroySurfaceKHR(m_surface, nullptr);
    ENGINE_DEBUG("Destroyed Surface");
  });

  m_physicalDevice = ChoosePhysicalDevice(m_instance);
  m_logicalDevice = CreateLogicalDevice(m_physicalDevice, m_surface, m_deviceDeletionQueue);
  u32 graphicsQueueFamilyIndex = FindQueueFamilyIndex(m_physicalDevice, m_surface, vk::QueueFlagBits::eGraphics);
  m_graphicsQueue = m_logicalDevice.getQueue(graphicsQueueFamilyIndex, 0);
}

Renderer::~Renderer() {
  while (m_deviceDeletionQueue.size() > 0) {
    m_deviceDeletionQueue.back()(m_logicalDevice);
    m_deviceDeletionQueue.pop_back();
  }
  while (m_instanceDeletionQueue.size() > 0) {
    m_instanceDeletionQueue.back()(m_instance);
    m_instanceDeletionQueue.pop_back();
  }
}
void Renderer::Init() {}

} // namespace Engine::Vulkan
