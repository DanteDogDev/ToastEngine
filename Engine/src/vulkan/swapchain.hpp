#pragma once
#include "src/vulkan/device.hpp"
#include <vulkan/vulkan_structs.hpp>
namespace Engine::Vulkan {
struct SurfaceDetails {
  vk::SurfaceCapabilitiesKHR capabilities;
  std::vector<vk::SurfaceFormatKHR> formats;
  std::vector<vk::PresentModeKHR> presentModes;
};
class Swapchain {
public:
  void Init(DeviceManager &devices, vk::SurfaceKHR surface, u32 width, u32 height);

  u32 m_imageCount;
  vk::SwapchainKHR m_chain;
  vk::SurfaceFormatKHR m_format;
  vk::Extent2D m_extent;

private:
  SurfaceDetails QuerySurfaceSupport(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
  vk::Extent2D ChooseExtent(vk::SurfaceCapabilitiesKHR capabilities, u32 width, u32 height);
  vk::PresentModeKHR ChoosePresentMode(std::vector<vk::PresentModeKHR> presentModes);
  vk::SurfaceFormatKHR ChooseSurfaceFormat(std::vector<vk::SurfaceFormatKHR> formats);
};
} // namespace Engine::Vulkan
