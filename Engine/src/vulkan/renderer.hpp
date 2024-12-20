#pragma once
#include <deque>
#include <functional>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_handles.hpp>
namespace Engine::Vulkan {
class Renderer {
public:
  Renderer();
  ~Renderer();
  void Init();

private:
  vk::Instance m_instance;
  vk::DispatchLoaderDynamic m_dldi;
  vk::PhysicalDevice m_physicalDevice;
  vk::Device m_logicalDevice;
  vk::DebugUtilsMessengerEXT m_debugMessenger;
  std::deque<std::function<void(vk::Device)>> m_deviceDeletionQueue;
  std::deque<std::function<void(vk::Instance)>> m_instanceDeletionQueue;
};
} // namespace Engine::Vulkan
