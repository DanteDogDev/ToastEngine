#include "device.hpp"
#include "src/core/log.hpp"
#include "src/vulkan/vklog.hpp"
#include "vulkanConfig.hpp"
using namespace Engine::Vulkan;
void DeviceManager::Init(vk::Instance intance, vk::SurfaceKHR surface) {
  ENGINE_INFO("Creating Devices Object")
  m_instance = intance;
  m_surface = surface;
  ChoosePhysicalDevice();
  FindQueueFamily();
  CreateLogicalDevice();
}
DeviceManager::~DeviceManager() {
  ENGINE_INFO("Deleting Devices Object");
  while (m_deviceDeletionQueue.size() > 0) {
    m_deviceDeletionQueue.back()(m_logicalDevice);
    m_deviceDeletionQueue.pop_back();
  }
}

bool DeviceManager::IsDeviceSuitable(vk::PhysicalDevice device) {
  ENGINE_DEBUG("Checking if Device is suitable: {}", device.getProperties().deviceName.data());

  ENGINE_DEBUG("Physical Device Extensions to be requested");
  LogList(VulkanConfig::physicalDeviceExtensions);
  ENGINE_DEBUG("Physical Device can support these Extensions")
  std::vector<vk::ExtensionProperties> supportedExtensions = device.enumerateDeviceExtensionProperties();
  LogList(supportedExtensions);

  for (const char *request : VulkanConfig::physicalDeviceExtensions) {
    bool found = false;
    for (const auto &support : supportedExtensions) {
      if (strcmp(support.extensionName, request) == 0) {
        ENGINE_DEBUG("Extension \"{}\" Supported", request);
        found = true;
        break;
      }
    }
    if (!found) {
      ENGINE_WARN("Extension \"{}\" Not Supported", request);
    }
  }
  return true;
}
void DeviceManager::ChoosePhysicalDevice() {
  ENGINE_DEBUG("Choosing Physical Device");
  std::vector<vk::PhysicalDevice> devices = m_instance.enumeratePhysicalDevices();
  std::vector<vk::PhysicalDeviceType> types = {vk::PhysicalDeviceType::eDiscreteGpu, vk::PhysicalDeviceType::eIntegratedGpu,
                                               vk::PhysicalDeviceType::eVirtualGpu, vk::PhysicalDeviceType::eCpu, vk::PhysicalDeviceType::eOther};
  for (const auto &type : types) {
    for (const auto &device : devices) {
      auto properties = device.getProperties();
      if (properties.deviceType == type && IsDeviceSuitable(device)) {
        ENGINE_INFO("Chosen Device:{}", device.getProperties().deviceName.data());
        m_physicalDevice = device;
        return;
      }
    }
  }
  ENGINE_CRITICAL("NO SUITABLE PHYSICAL DEVICE FOUND");
}

void DeviceManager::FindQueueFamily() {
  ENGINE_DEBUG("Finding Queue Familys");
  std::vector<vk::QueueFamilyProperties> queueFamilys = m_physicalDevice.getQueueFamilyProperties();
  LogQueues(queueFamilys);
  int i = 0;
  for (const auto &queueFamily : queueFamilys) {
    if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
      ENGINE_DEBUG("Found eGraphics queue");
      m_queueFamilyIndices.graphicsFamilies.push_back(i);
    }
    if (queueFamily.queueFlags & vk::QueueFlagBits::eCompute) {
      ENGINE_DEBUG("Found eCompute queue");
      m_queueFamilyIndices.computeFamilies.push_back(i);
    }
    if (m_physicalDevice.getSurfaceSupportKHR(i, m_surface)) {
      ENGINE_DEBUG("Found surface queue");
      m_queueFamilyIndices.computeFamilies.push_back(i);
    }
    i++;
  }
}

void DeviceManager::CreateLogicalDevice() {
  ENGINE_INFO("Creating Logical Device");

  vk::DeviceQueueCreateInfo queueInfo{};
  queueInfo.flags = vk::DeviceQueueCreateFlags();
  queueInfo.queueFamilyIndex = m_queueFamilyIndices.graphicsFamilies[0];
  queueInfo.queueCount = 1;
  float queuePriority = 1.0f;
  queueInfo.pQueuePriorities = &queuePriority;

  vk::PhysicalDeviceFeatures deviceFeatures = vk::PhysicalDeviceFeatures();

  vk::DeviceCreateInfo deviceInfo{};
  deviceInfo.pQueueCreateInfos = &queueInfo;
  deviceInfo.queueCreateInfoCount = 1;
  deviceInfo.pEnabledFeatures = &deviceFeatures;
  deviceInfo.enabledExtensionCount = 0;
  deviceInfo.ppEnabledExtensionNames = nullptr;

  vk::Device logicalDevice = m_physicalDevice.createDevice(deviceInfo);
  m_deviceDeletionQueue.push_back([](vk::Device device) {
    device.destroy();
    ENGINE_DEBUG("Deleted logical Device");
  });
  m_logicalDevice = logicalDevice;
}
