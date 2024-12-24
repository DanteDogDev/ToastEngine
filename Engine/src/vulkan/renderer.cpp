#include "renderer.hpp"
using namespace Engine;
VulkanEngine::VulkanEngine(GLFWwindow *window) {
  m_instanceManager.Init();
  m_surface = m_instanceManager.MakeSurface(window);
  m_deviceManager.Init(m_instanceManager.m_instance, m_surface);

  int width, height;
  glfwGetWindowSize(window, &width, &height);
  m_swapchain.Init(m_deviceManager, m_surface, width, height);
  std::vector<vk::Image> images = m_deviceManager.m_logicalDevice.getSwapchainImagesKHR(m_swapchain.m_chain);
  for (vk::Image image : images) {
    m_frames.push_back(Vulkan::Frame(image, m_deviceManager, m_swapchain.m_format.format));
  }

  // shaders = MakeShaderObjects(m_deviceManager, m_instanceManager.m_dldi, "bin/Resources/shaders/vertex.spv", "bin/Resources/shaders/fragment.spv");

  m_pipeline.Init(m_deviceManager, m_swapchain);
}
void VulkanEngine::Draw() {}
VulkanEngine::~VulkanEngine() {}
