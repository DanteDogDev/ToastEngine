#pragma once
#include "device.hpp"
#include "instance.hpp"
namespace Engine::Vulkan {
class Renderer {
public:
  Renderer(GLFWwindow *window);
  ~Renderer();

private:
  InstanceManager m_instanceManager;
  DeviceManager m_deviceManager;
  vk::SurfaceKHR m_surface;
};
} // namespace Engine::Vulkan
