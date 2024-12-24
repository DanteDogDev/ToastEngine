#include "frame.hpp"
#include "image.hpp"
using namespace Engine::Vulkan;

Frame::Frame(vk::Image image, DeviceManager &devices, vk::Format swapchainFormat) : m_image(image) {
  m_imageView = CreateImageView(devices.m_logicalDevice, m_image, swapchainFormat);
  VkImageView imageViewHandler = m_imageView;
  devices.m_deviceDeletionQueue.push_back([imageViewHandler](vk::Device device) {
    ENGINE_DEBUG("Destroying ImageView");
    device.destroyImageView(imageViewHandler);
  });
}
