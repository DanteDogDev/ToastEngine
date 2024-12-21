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
    std::vector<u32> graphicsFamilies;
    std::vector<u32> computeFamilies;
    std::vector<u32> presentFamilies;
  } m_queueFamilyIndices;

  void ChoosePhysicalDevice();
  bool IsDeviceSuitable(vk::PhysicalDevice device);
  void FindQueueFamily();
  void CreateLogicalDevice();
};
} // namespace Engine::Vulkan
