#include "frame.hpp"
#include "image.hpp"
#include "src/vulkan/device.hpp"
#include "src/vulkan/swapchain.hpp"
using namespace Engine::Vulkan;

void FrameManager::Init(DeviceManager &devices, Swapchain &swapchain, vk::RenderPass renderPass) {
  m_images = devices.m_logicalDevice.getSwapchainImagesKHR(swapchain.m_chain);
  // Creates Image Views
  for (vk::Image image : m_images) {
    VkImageView imageView = CreateImageView(devices.m_logicalDevice, image, swapchain.m_format.format);
    m_imageViews.push_back(imageView);
    devices.m_deviceDeletionQueue.push_back([imageView](vk::Device device) {
      ENGINE_DEBUG("Destroying ImageView");
      device.destroyImageView(imageView);
    });
  }

  // Creates FrameBuffers
  vk::FramebufferCreateInfo bufferInfo{};
  bufferInfo.setRenderPass(renderPass);
  bufferInfo.setAttachmentCount(1);
  bufferInfo.setWidth((u32)swapchain.m_extent.width);
  bufferInfo.setHeight((u32)swapchain.m_extent.height);
  bufferInfo.setLayers(1);
  for (u32 i = 0; i < m_images.size(); i++) {
    try {
      bufferInfo.setPAttachments(&m_imageViews[i]);
      vk::Framebuffer frameBuffer = devices.m_logicalDevice.createFramebuffer(bufferInfo);
      VkFramebuffer bufferHandler = frameBuffer;
      m_frameBuffers.push_back(frameBuffer);
      devices.m_deviceDeletionQueue.push_back([bufferHandler](vk::Device device) {
        ENGINE_DEBUG("Destroyed FrameBuffer");
        device.destroyFramebuffer(bufferHandler);
      });
    } catch (const vk::SystemError &err) {
      ENGINE_CRITICAL("Failed to create Vulkan Frame Buffer: {}", err.what());
    }
  }
}
