#include "renderer.hpp"
#include "GLFW/glfw3.h"
#include "device.hpp"

namespace Engine::Vulkan {
Renderer::Renderer(GLFWwindow *window) //
    : m_instanceManager() {
  m_surface = m_instanceManager.MakeSurface(window);

  m_physicalDevice = ChoosePhysicalDevice(m_instanceManager.m_instance);
  m_logicalDevice = CreateLogicalDevice(m_physicalDevice, m_surface, m_deviceDeletionQueue);
  u32 graphicsQueueFamilyIndex = FindQueueFamilyIndex(m_physicalDevice, m_surface, vk::QueueFlagBits::eGraphics);
  m_graphicsQueue = m_logicalDevice.getQueue(graphicsQueueFamilyIndex, 0);
}

Renderer::~Renderer() {
  while (m_deviceDeletionQueue.size() > 0) {
    m_deviceDeletionQueue.back()(m_logicalDevice);
    m_deviceDeletionQueue.pop_back();
  }
}
void Renderer::Init() {}

} // namespace Engine::Vulkan
