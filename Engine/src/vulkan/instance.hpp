namespace Engine::Vulkan {
class Instance {
public:
  Instance();
  ~Instance();

  vk::SurfaceKHR MakeSurface(GLFWwindow *window);

  vk::Instance m_instance;
  std::deque<std::function<void(vk::Instance)>> m_instanceDeletionQueue;

private:
  vk::DispatchLoaderDynamic m_dldi;
  vk::DebugUtilsMessengerEXT m_debugMessenger;

  void MakeInstance(const char *appName);
  void AddRequiredExtensions();
  void SupportedByInstance();

  void MakeDebugMessenger();
};
} // namespace Engine::Vulkan
