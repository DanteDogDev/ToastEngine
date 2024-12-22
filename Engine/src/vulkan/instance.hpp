#pragma once
namespace Engine::Vulkan {
class InstanceManager {
public:
  void Init();
  ~InstanceManager();

  vk::SurfaceKHR MakeSurface(GLFWwindow *window);

  vk::Instance m_instance;
  std::deque<std::function<void(vk::Instance)>> m_instanceDeletionQueue;

private:
  vk::DispatchLoaderDynamic m_dldi;
  vk::DebugUtilsMessengerEXT m_debugMessenger;

  vk::Instance MakeInstance(const char *appName);
  void AddRequiredExtensions();
  void SupportedByInstance();

  void MakeDebugMessenger();
};
} // namespace Engine::Vulkan
