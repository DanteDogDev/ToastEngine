#pragma once
#include "src/vulkan/device.hpp"
#include <vulkan/vulkan_handles.hpp>
namespace Engine::Vulkan {
class Frame {
public:
  Frame(vk::Image image, DeviceManager &devices, vk::Format swapchainFormat);

  vk::Image m_image;
  vk::ImageView m_imageView;

private:
};
} // namespace Engine::Vulkan
