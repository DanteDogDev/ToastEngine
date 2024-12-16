#include "defines.h"
#include <vulkan/vulkan_core.h>
namespace Engine::Renderer {

class InstanceManager {
public:
  InstanceManager();
  ~InstanceManager();

private:
  VkInstance m_instance;
  void CreateInstance();
};
} // namespace Engine::Renderer
