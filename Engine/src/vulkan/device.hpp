#pragma once
#include <vulkan/vulkan_handles.hpp>
struct QueueFamilyIndices {
  std::vector<u32> graphicsFamilies;
  std::vector<u32> computeFamilies;
  std::vector<u32> presentFamilies;
};
namespace Engine::Vulkan {
class DeviceManager {
public:
  void Init(vk::Instance, vk::SurfaceKHR surface);
  ~DeviceManager();

  vk::PhysicalDevice m_physicalDevice;
  vk::Device m_logicalDevice;
  vk::Queue m_graphicsQueue;
  std::deque<std::function<void(vk::Device)>> m_deviceDeletionQueue;
  QueueFamilyIndices m_queueFamilyIndices;

private:
  vk::PhysicalDevice ChoosePhysicalDevice(vk::Instance instance);
  bool IsDeviceSuitable(vk::PhysicalDevice device);
  QueueFamilyIndices FindQueueFamily(vk::SurfaceKHR surface);
  vk::Device CreateLogicalDevice();
};
} // namespace Engine::Vulkan
