#pragma once
namespace Engine::Vulkan {
class InstanceManager {
public:
  void Init();
  ~InstanceManager();

  vk::SurfaceKHR MakeSurface(GLFWwindow *window);

  vk::Instance m_instance;
  vk::DispatchLoaderDynamic m_dldi;
  std::deque<std::function<void(vk::Instance)>> m_instanceDeletionQueue;

private:
  vk::DebugUtilsMessengerEXT m_debugMessenger;

  vk::Instance MakeInstance(const char *appName);
  void AddRequiredExtensions();
  void SupportedByInstance();

  void MakeDebugMessenger();
};
} // namespace Engine::Vulkan
