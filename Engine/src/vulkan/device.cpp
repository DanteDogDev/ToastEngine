#include "device.hpp"
#include "src/core/log.hpp"
#include "vklog.hpp"
#include <vector>

bool Supports(const vk::PhysicalDevice &device, const char **ppRequestedExtensions, const u32 requestedExtensionCount) {
  ENGINE_DEBUG("Requested Physical Device Extensions");
  LogList(ppRequestedExtensions, requestedExtensionCount);

  std::vector<vk::ExtensionProperties> extensions = device.enumerateDeviceExtensionProperties();
  ENGINE_DEBUG("Physical Device supported extensions");
  LogExtensions(extensions);

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
