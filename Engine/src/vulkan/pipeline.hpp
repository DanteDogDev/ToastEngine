#pragma once
#include "src/vulkan/device.hpp"
#include "src/vulkan/swapchain.hpp"
#include <vulkan/vulkan_handles.hpp>
namespace Engine::Vulkan {
class PipelineManager {
public:
  void Init(DeviceManager &devices, Swapchain &swapchain);
  vk::Pipeline m_graphicsPipeline;
  vk::RenderPass m_renderPass;

private:
  vk::Pipeline MakeGraphicsPipeline(DeviceManager &devices, Swapchain &swapchain);
  vk::RenderPass MakeRenderPass(DeviceManager &devices, Swapchain &swapchain);
};
} // namespace Engine::Vulkan
