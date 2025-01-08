#pragma once
#include "src/vulkan/device.hpp"
#include "src/vulkan/swapchain.hpp"
namespace Engine::Vulkan {
class FrameManager {
public:
  std::vector<vk::Image> m_images;
  std::vector<vk::ImageView> m_imageViews;
  std::vector<vk::Framebuffer> m_frameBuffers;

  void CreateFrameBuffer(DeviceManager &devices, std::vector<vk::ImageView> imageViews, vk::RenderPass renderPass, vk::Extent2D swapchainExtent);
  void Init(DeviceManager &devices, Swapchain &swapchain, vk::RenderPass renderPass);

private:
};
} // namespace Engine::Vulkan
