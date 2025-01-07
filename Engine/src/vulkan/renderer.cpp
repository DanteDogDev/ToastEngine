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
  std::vector<vk::Image> images = m_deviceManager.m_logicalDevice.getSwapchainImagesKHR(m_swapchain.m_chain);
  for (vk::Image image : images) {
    m_frames.push_back(Vulkan::Frame(image, m_deviceManager, m_swapchain.m_format.format));
  }
  m_pipeline.Init(m_deviceManager, m_swapchain);

  std::vector<vk::ImageView> imageViews;
  for (Vulkan::Frame frame : m_frames) {
    imageViews.push_back(frame.m_imageView);
  }
  for (Vulkan::Frame frame : m_frames) {
    frame.CreateFrameBuffer(m_deviceManager, imageViews, m_pipeline.m_renderPass, m_swapchain.m_extent);
  }
}
void VulkanEngine::Draw() {}
VulkanEngine::~VulkanEngine() {}
