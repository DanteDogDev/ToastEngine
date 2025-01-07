#include "frame.hpp"
#include "image.hpp"
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_structs.hpp>
using namespace Engine::Vulkan;

Frame::Frame(vk::Image image, DeviceManager &devices, vk::Format swapchainFormat) : m_image(image) {
  // Creates Image View
  m_imageView = CreateImageView(devices.m_logicalDevice, m_image, swapchainFormat);
  VkImageView imageViewHandler = m_imageView;
  devices.m_deviceDeletionQueue.push_back([imageViewHandler](vk::Device device) {
    ENGINE_DEBUG("Destroying ImageView");
    device.destroyImageView(imageViewHandler);
  });
}

void Frame::CreateFrameBuffer(DeviceManager &devices, std::vector<vk::ImageView> imageViews, vk::RenderPass renderPass,
                              vk::Extent2D swapchainExtent) {
  vk::FramebufferCreateInfo bufferInfo{};
  bufferInfo.setRenderPass(renderPass);
  bufferInfo.setAttachmentCount(1);
  bufferInfo.setPAttachments(imageViews.data());
  bufferInfo.setWidth((u32)swapchainExtent.width);
  bufferInfo.setHeight((u32)swapchainExtent.height);
  bufferInfo.setLayers(1);

  try {
    m_frameBuffer = devices.m_logicalDevice.createFramebuffer(bufferInfo);
    VkFramebuffer bufferHandler = m_frameBuffer;
    devices.m_deviceDeletionQueue.push_back([bufferHandler](vk::Device device) {
      ENGINE_DEBUG("Destroyed FrameBuffer");
      device.destroyFramebuffer(bufferHandler);
    });
  } catch (const vk::SystemError &err) {
    ENGINE_CRITICAL("Failed to create Vulkan Frame Buffer: {}", err.what());
  }
}
