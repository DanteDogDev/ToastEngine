#include "defines.h"
#include <vulkan/vulkan_core.h>
#pragma once
namespace Engine::Renderer {
#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class InstanceManager {
public:
  ~InstanceManager();
  void CreateInstance();

private:
  VkInstance m_instance;
};
} // namespace Engine::Renderer
