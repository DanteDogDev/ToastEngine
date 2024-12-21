#include "device.hpp"
#include "instance.hpp"
#include "vklog.hpp"
#include "vulkanConfig.hpp"
bool Supports(const vk::PhysicalDevice &device, const char **ppRequestedExtensions, const u32 requestedExtensionCount) {
  ENGINE_DEBUG("Requested Physical Device Extensions");
  LogList(ppRequestedExtensions, requestedExtensionCount);

  std::vector<vk::ExtensionProperties> extensions = device.enumerateDeviceExtensionProperties();
  ENGINE_DEBUG("Physical Device supported extensions");
  LogList(extensions);

  for (u32 i = 0; i < requestedExtensionCount; ++i) {
    bool supported = false;

    for (vk::ExtensionProperties &extension : extensions) {
      std::string name = extension.extensionName;
      if (!name.compare(ppRequestedExtensions[i])) {
        supported = true;
        break;
      }
    }
    if (!supported) {
      ENGINE_DEBUG("Device not supported:{}", device.getProperties().deviceName.data());
      return false;
    }
  }

  return true;
}

bool IsSuitable(const vk::PhysicalDevice &device) {
  ENGINE_DEBUG("Checking if Device is suitable");
  // Requested Extensions
  const char *ppRequestedExtension = VK_KHR_SWAPCHAIN_EXTENSION_NAME;

  if (Supports(device, &ppRequestedExtension, 1)) {
    ENGINE_DEBUG("Device can support the requested extensions!");
  } else {
    ENGINE_DEBUG("Device can't support the requested extensions!");
    return false;
  }
  return true;
}

vk::PhysicalDevice ChoosePhysicalDevice(const vk::Instance &instance) {
  ENGINE_INFO("Choosing Physical Device");
  std::vector<vk::PhysicalDevice> availableDevices = instance.enumeratePhysicalDevices();
  for (vk::PhysicalDevice device : availableDevices) {

    if (IsSuitable(device)) {
      ENGINE_INFO("Chosen Device:{}", device.getProperties().deviceName.data());
      return device;
    }
  }
  ENGINE_CRITICAL("NO SUITABLE PHYSICAL DEVICE");
  return availableDevices[0];
}

u32 FindQueueFamilyIndex(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, vk::QueueFlags queueType) {
  ENGINE_DEBUG("Finding queue family index");
  std::vector<vk::QueueFamilyProperties> queueFamilys = physicalDevice.getQueueFamilyProperties();
  LogQueues(queueFamilys);
  for (u32 i = 0; i < queueFamilys.size(); i++) {
    vk::QueueFamilyProperties queueFamily = queueFamilys[i];
    bool canPresent = false;
    if (surface) {
      if (physicalDevice.getSurfaceSupportKHR(i, surface)) {
        canPresent = true;
      }
    } else {
      canPresent = true;
    }

    bool supported = false;
    // Queue Flags is a bitmask
    if (queueFamily.queueFlags & queueType) {
      supported = true;
    }

    if (supported && canPresent) {
      return i;
    }
  }
  return UINT32_MAX;
}

vk::Device CreateLogicalDevice(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface,
                               std::deque<std::function<void(vk::Device)>> &deletionQueue) {
  ENGINE_INFO("Creating Logical Device");
  u32 graphicsIndex = FindQueueFamilyIndex(physicalDevice, surface, vk::QueueFlagBits::eGraphics);
  float queuePriority = 1.0f;
  vk::DeviceQueueCreateInfo queueInfo{};

  queueInfo.flags = vk::DeviceQueueCreateFlags();
  queueInfo.queueFamilyIndex = graphicsIndex;
  queueInfo.queueCount = 1;
  queueInfo.pQueuePriorities = &queuePriority;
  queueInfo.pNext = nullptr;

  vk::PhysicalDeviceFeatures deviceFeatures = vk::PhysicalDeviceFeatures();

  vk::DeviceCreateInfo deviceInfo{};
  deviceInfo.queueCreateInfoCount = 1;
  deviceInfo.pQueueCreateInfos = &queueInfo;
  deviceInfo.enabledLayerCount = VulkanConfig::instanceLayers.size();
  deviceInfo.ppEnabledLayerNames = VulkanConfig::instanceLayers.data();
  deviceInfo.enabledExtensionCount = 0;
  deviceInfo.ppEnabledExtensionNames = nullptr;
  deviceInfo.pEnabledFeatures = &deviceFeatures;

  vk::Device logicalDevice = physicalDevice.createDevice(deviceInfo);
  deletionQueue.push_back([](vk::Device device) {
    device.destroy();
    ENGINE_DEBUG("Deleted logical Device");
  });
  return logicalDevice;
}
