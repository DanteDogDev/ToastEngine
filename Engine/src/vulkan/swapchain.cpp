#include "swapchain.hpp"
#include "src/core/log.hpp"
#include "src/vulkan/vklog.hpp"
#include <exception>
using namespace Engine::Vulkan;

void Swapchain::Init(DeviceManager &devices, vk::SurfaceKHR surface, u32 width, u32 height) {
  ENGINE_INFO("Creating Swapchain");
  SurfaceDetails support = QuerySurfaceSupport(devices.m_physicalDevice, surface);
  m_format = ChooseSurfaceFormat(support.formats);
  vk::PresentModeKHR presentMode = ChoosePresentMode(support.presentModes);
  m_extent = ChooseExtent(support.capabilities, width, height);
  m_imageCount = std::min(support.capabilities.maxImageCount, support.capabilities.minImageCount + 1);

  vk::SwapchainCreateInfoKHR createInfo{};
  createInfo.flags = vk::SwapchainCreateFlagsKHR();
  createInfo.surface = surface;
  createInfo.minImageCount = m_imageCount;
  createInfo.imageFormat = m_format.format;
  createInfo.imageColorSpace = m_format.colorSpace;
  createInfo.imageExtent = m_extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
  // Sharing Mode
  // eExclusive: An image is owned by one queue family at a time
  // eConcurrent: An image can be used across multiple queue familys
  u32 queueFamilyIndices[] = {devices.m_queueFamilyIndices.graphicsFamilies[0], devices.m_queueFamilyIndices.presentFamilies[0]};
  if (queueFamilyIndices[0] != queueFamilyIndices[1]) {
    ENGINE_DEBUG("SharingMode:eConcurrent"); // If graphics queue and present queue are different
    createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = queueFamilyIndices;
  } else {
    ENGINE_DEBUG("SharingMode:eExclusive"); // More performant
    createInfo.imageSharingMode = vk::SharingMode::eExclusive;
    createInfo.queueFamilyIndexCount = 0;
    createInfo.pQueueFamilyIndices = nullptr;
  }
  createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
  createInfo.preTransform = support.capabilities.currentTransform;
  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;
  createInfo.oldSwapchain = VK_NULL_HANDLE;

  try {
    m_chain = devices.m_logicalDevice.createSwapchainKHR(createInfo);
  } catch (const std::exception &e) {
    ENGINE_CRITICAL("Failed to create swapchain:{}", e.what());
  }
  devices.m_deviceDeletionQueue.push_back([this](vk::Device device) {
    ENGINE_DEBUG("Destroyed Swapchain");
    device.destroySwapchainKHR(m_chain);
  });
}

SurfaceDetails Swapchain::QuerySurfaceSupport(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface) {
  ENGINE_DEBUG("querying Surface Support");
  SurfaceDetails support;
  support.capabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
  LogList(support.capabilities);
  support.formats = physicalDevice.getSurfaceFormatsKHR(surface);
  LogList(support.formats);
  support.presentModes = physicalDevice.getSurfacePresentModesKHR(surface);
  LogList(support.presentModes);
  return support;
}

vk::Extent2D Swapchain::ChooseExtent(vk::SurfaceCapabilitiesKHR capabilities, u32 width, u32 height) {
  ENGINE_DEBUG("Choosing Extent");
  if (capabilities.currentExtent.width != UINT32_MAX) {
    return capabilities.currentExtent;
  }
  vk::Extent2D extent;
  extent.width = std::min(                                 //
      capabilities.maxImageExtent.width,                   //
      std::max(capabilities.minImageExtent.width, width)   //
  );                                                       //
  extent.height = std::min(                                //
      capabilities.maxImageExtent.height,                  //
      std::max(capabilities.minImageExtent.height, height) //
  );                                                       //
  return extent;
}

vk::PresentModeKHR Swapchain::ChoosePresentMode(std::vector<vk::PresentModeKHR> presentModes) {
  ENGINE_DEBUG("Choosing Present Mode");
  for (auto mode : presentModes) {
    if (mode == vk::PresentModeKHR::eMailbox) {
      ENGINE_INFO("Chosen Present Mode: eMailbox");
      return mode;
    }
  }
  ENGINE_INFO("Chosen Present Mode: eFifo");
  return vk::PresentModeKHR::eFifo;
}

vk::SurfaceFormatKHR Swapchain::ChooseSurfaceFormat(std::vector<vk::SurfaceFormatKHR> formats) {
  ENGINE_DEBUG("Choosing Surface Format");
  for (auto format : formats) {
    if (format == vk::Format::eB8G8R8A8Unorm && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
      ENGINE_INFO("Chosen Format: Supported Pixel Format: {}, supported color space: {}", vk::to_string(format.format),
                  vk::to_string(format.colorSpace));
      return format;
    }
  }
  return formats[0];
}
