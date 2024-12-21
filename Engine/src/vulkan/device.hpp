namespace Engine::Vulkan {
class DeviceManager {
public:
  void Init(vk::Instance, vk::SurfaceKHR surface);
  ~DeviceManager();

private:
  vk::Instance m_instance;
  vk::SurfaceKHR m_surface;

  vk::PhysicalDevice m_physicalDevice;
  vk::Device m_logicalDevice;
  vk::Queue m_graphicsQueue;
  std::deque<std::function<void(vk::Device)>> m_deviceDeletionQueue;

  struct {
    u32 graphicsFamily;
    u32 computeFamily;
  } m_queueFamilyIndices;

  void ChoosePhysicalDevice();
  bool IsDeviceSuitable(vk::PhysicalDevice device);
  void FindQueueFamily();
  void CreateLogicalDevice();
};
} // namespace Engine::Vulkan
