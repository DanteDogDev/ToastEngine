#pragma once
#include <vulkan/vulkan_handles.hpp>
namespace Engine::Vulkan {
class Renderer {
public:
  Renderer(GLFWwindow *window);
  ~Renderer();
  void Init();

private:
  vk::Instance m_instance;
  vk::PhysicalDevice m_physicalDevice;
  vk::Device m_logicalDevice;
  vk::Queue m_graphicsQueue;

  vk::SurfaceKHR m_surface;

  vk::DispatchLoaderDynamic m_dldi;
  vk::DebugUtilsMessengerEXT m_debugMessenger;

  std::deque<std::function<void(vk::Device)>> m_deviceDeletionQueue;
  std::deque<std::function<void(vk::Instance)>> m_instanceDeletionQueue;
};
} // namespace Engine::Vulkan
