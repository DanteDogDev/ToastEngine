#include "renderer.hpp"
#include "src/vulkan/frame.hpp"
using namespace Engine;
VulkanEngine::VulkanEngine(GLFWwindow *window) {
  m_instanceManager.Init();
  m_surface = m_instanceManager.MakeSurface(window);
  m_deviceManager.Init(m_instanceManager.m_instance, m_surface);

  int width, height;
  glfwGetWindowSize(window, &width, &height);

  m_swapchain.Init(m_deviceManager, m_surface, width, height);
  m_pipeline.Init(m_deviceManager, m_swapchain);

  m_frameManager.Init(m_deviceManager, m_swapchain, m_pipeline.m_renderPass);
}
void VulkanEngine::Draw() {}
VulkanEngine::~VulkanEngine() {}
