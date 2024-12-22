#pragma once
#include "device.hpp"
#include "frame.hpp"
#include "instance.hpp"
#include "swapchain.hpp"
namespace Engine::Vulkan {
class Renderer {
public:
  Renderer(GLFWwindow *window);
  ~Renderer();

private:
  InstanceManager m_instanceManager;
  DeviceManager m_deviceManager;
  vk::SurfaceKHR m_surface;

  Swapchain m_swapchain;
  std::vector<Frame> m_frames;
};
} // namespace Engine::Vulkan
