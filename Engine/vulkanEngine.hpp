#include "src/core/window.hpp"
#include "src/vulkan/instanceManager.hpp"
#pragma once
using namespace Engine;
class VulkanEngine {
public:
  VulkanEngine();
  void Init();
  void Run();
  void Cleanup();

private:
  Core::Window m_window;
  Renderer::InstanceManager InstanceManager;
};
int test();
