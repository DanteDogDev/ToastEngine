#include "renderer.hpp"
using namespace Engine::Vulkan;
Renderer::Renderer(GLFWwindow *window) {
  m_instanceManager.Init();
  m_surface = m_instanceManager.MakeSurface(window);
  m_deviceManager.Init(m_instanceManager.m_instance, m_surface);
  int width, height;
  glfwGetWindowSize(window, &width, &height);
  m_swapchain.Init(m_deviceManager, m_surface, width, height);
  std::vector<vk::Image> images = m_deviceManager.m_logicalDevice.getSwapchainImagesKHR(m_swapchain.m_chain);
  for (vk::Image image : images) {
    m_frames.push_back(Frame(image, m_deviceManager, m_swapchain.m_format.format));
  }
}
Renderer::~Renderer() {}
