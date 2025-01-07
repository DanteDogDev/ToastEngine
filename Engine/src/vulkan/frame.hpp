#pragma once
#include "src/vulkan/device.hpp"
// MAKE CLASS THAT HOLD ARRAYS INSTEAD OF A SINGLE FRAME ALL FRAMES
namespace Engine::Vulkan {
class Frame {
public:
  Frame(vk::Image image, DeviceManager &devices, vk::Format swapchainFormat);

  vk::Image m_image;
  vk::ImageView m_imageView;
  vk::Framebuffer m_frameBuffer;

  void CreateFrameBuffer(DeviceManager &devices, std::vector<vk::ImageView> imageViews, vk::RenderPass renderPass, vk::Extent2D swapchainExtent);

private:
};
} // namespace Engine::Vulkan
