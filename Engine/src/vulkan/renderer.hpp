#pragma once
#include "instance.hpp"
namespace Engine::Vulkan {
class Renderer {
public:
  Renderer(GLFWwindow *window);
  ~Renderer();
  void Init();

private:
  Instance m_instanceManager;
  vk::SurfaceKHR m_surface;

  vk::PhysicalDevice m_physicalDevice;
  vk::Device m_logicalDevice;
  vk::Queue m_graphicsQueue;
  std::deque<std::function<void(vk::Device)>> m_deviceDeletionQueue;
};
} // namespace Engine::Vulkan
