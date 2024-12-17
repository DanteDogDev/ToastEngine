#include <deque>
#include <functional>
#include <vulkan/vulkan.hpp>
namespace Engine::Vulkan {
class Renderer {
public:
  Renderer();
  ~Renderer();
  void Init();

private:
  vk::Instance m_instance;
  std::deque<std::function<void(vk::Instance)>> m_deletionQueue;
  vk::DispatchLoaderDynamic m_dldi;
  vk::DebugUtilsMessengerEXT m_debugMessenger;
};
} // namespace Engine::Vulkan
