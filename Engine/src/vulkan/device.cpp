#include "device.hpp"
#include "src/vulkan/vklog.hpp"
#include "vulkanConfig.hpp"
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_structs.hpp>
using namespace Engine::Vulkan;
void DeviceManager::Init(vk::Instance instance, vk::SurfaceKHR surface) {
  ENGINE_INFO("Creating Devices Object")
  m_physicalDevice = ChoosePhysicalDevice(instance);
  m_queueFamilyIndices = FindQueueFamily(surface);
  m_logicalDevice = CreateLogicalDevice();
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

vk::PhysicalDevice DeviceManager::ChoosePhysicalDevice(vk::Instance instance) {
  ENGINE_DEBUG("Choosing Physical Device");
  std::vector<vk::PhysicalDevice> devices = instance.enumeratePhysicalDevices();
  std::vector<vk::PhysicalDeviceType> types = {vk::PhysicalDeviceType::eDiscreteGpu, vk::PhysicalDeviceType::eIntegratedGpu,
                                               vk::PhysicalDeviceType::eVirtualGpu, vk::PhysicalDeviceType::eCpu, vk::PhysicalDeviceType::eOther};
  for (const auto &type : types) {
    for (const auto &device : devices) {
      auto properties = device.getProperties();
      if (properties.deviceType == type && IsDeviceSuitable(device)) {
        ENGINE_INFO("Chosen Device:{}", device.getProperties().deviceName.data());
        return device;
      }
    }
  }
  ENGINE_CRITICAL("NO SUITABLE PHYSICAL DEVICE FOUND");
}

QueueFamilyIndices DeviceManager::FindQueueFamily(vk::SurfaceKHR surface) {
  ENGINE_DEBUG("Finding Queue Familys");
  std::vector<vk::QueueFamilyProperties> queueFamilys = m_physicalDevice.getQueueFamilyProperties();
  LogQueues(queueFamilys);
  QueueFamilyIndices queueFamilyIndices;
  int i = 0;
  for (const auto &queueFamily : queueFamilys) {
    if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
      ENGINE_DEBUG("Found eGraphics queue");
      queueFamilyIndices.graphicsFamilies.push_back(i);
    }
    if (queueFamily.queueFlags & vk::QueueFlagBits::eCompute) {
      ENGINE_DEBUG("Found eCompute queue");
      queueFamilyIndices.computeFamilies.push_back(i);
    }
    if (m_physicalDevice.getSurfaceSupportKHR(i, surface)) {
      ENGINE_DEBUG("Found surface queue");
      queueFamilyIndices.presentFamilies.push_back(i);
    }
    i++;
  }
  return queueFamilyIndices;
}

vk::Device DeviceManager::CreateLogicalDevice() {
  ENGINE_INFO("Creating Logical Device");

  vk::DeviceQueueCreateInfo queueInfo{};
  queueInfo.setFlags(vk::DeviceQueueCreateFlags());
  queueInfo.setQueueFamilyIndex(m_queueFamilyIndices.graphicsFamilies[0]);
  queueInfo.setQueueCount(1);
  float queuePriority = 1.0f;
  queueInfo.setPQueuePriorities(&queuePriority);

  vk::PhysicalDeviceFeatures deviceFeatures = vk::PhysicalDeviceFeatures();

  vk::DeviceCreateInfo deviceInfo{};
  deviceInfo.setPQueueCreateInfos(&queueInfo);
  deviceInfo.setQueueCreateInfoCount(1);
  deviceInfo.setPEnabledFeatures(&deviceFeatures);
  deviceInfo.setEnabledExtensionCount((u32)VulkanConfig::physicalDeviceExtensions.size());
  deviceInfo.setPpEnabledExtensionNames(VulkanConfig::physicalDeviceExtensions.data());

  vk::Device logicalDevice;
  try {
    logicalDevice = m_physicalDevice.createDevice(deviceInfo);
  } catch (const std::exception &e) {
    ENGINE_CRITICAL("Failed to create Logical Device:{}", e.what());
  }
  m_deviceDeletionQueue.push_back([](vk::Device device) {
    device.destroy();
    ENGINE_DEBUG("Deleted logical Device");
  });
  return logicalDevice;
}
