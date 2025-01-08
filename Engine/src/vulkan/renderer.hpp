#pragma once
#include "device.hpp"
#include "frame.hpp"
#include "instance.hpp"
#include "pipeline.hpp"
#include "swapchain.hpp"
namespace Engine {
class VulkanEngine {
public:
  VulkanEngine(GLFWwindow *window);
  ~VulkanEngine();
  void Draw();

private:
  Vulkan::InstanceManager m_instanceManager;
  Vulkan::DeviceManager m_deviceManager;
  Vulkan::Swapchain m_swapchain;
  Vulkan::PipelineManager m_pipeline;
  Vulkan::FrameManager m_frameManager;

  vk::SurfaceKHR m_surface;
  std::vector<Vulkan::FrameManager> m_frames;
  std::vector<vk::ShaderEXT> shaders;
};
} // namespace Engine
