#include "vklog.hpp"
#include "src/core/log.hpp"
void LogList(const char **list, u32 size) {
  for (u32 i = 0; i < size; i++) {
    ENGINE_DEBUG("\t\"{}\"", list[i]);
  }
}
void LogList(std::vector<vk::ExtensionProperties> &extensions) {
  for (const vk::ExtensionProperties &extension : extensions) {
    ENGINE_DEBUG("\t\'{}\'", std::string(extension.extensionName.data()));
  }
}
void LogList(std::vector<vk::LayerProperties> &layers) {
  for (const vk::LayerProperties &layer : layers) {
    ENGINE_DEBUG("\t\'{}\'", std::string(layer.layerName.data()));
  }
}
// FINAL LOGGING METHODS
void LogList(const std::vector<const char *> &list) {
  for (const char *token : list) {
    ENGINE_DEBUG("\t{}", token);
  }
}
void LogVersion(u32 version) {
  ENGINE_DEBUG(                                                                 //
      "System can support Vulkan Variant: {}, Major: {}, Minor: {}, Patch: {}", //
      vk::apiVersionVariant(version),                                           //
      vk::apiVersionMajor(version),                                             //
      vk::apiVersionMinor(version),                                             //
      vk::apiVersionPatch(version)                                              //
  );
}
void Log(std::vector<vk::ExtensionProperties> &extensions) {
  for (const vk::ExtensionProperties &extension : extensions) {
    ENGINE_DEBUG("\t{}", std::string(extension.extensionName.data()));
  }
}
void Log(std::vector<vk::LayerProperties> &layers) {
  for (const vk::LayerProperties &layer : layers) {
    ENGINE_DEBUG("\t{}", std::string(layer.layerName.data()));
  }
}

void LogQueues(const std::vector<vk::QueueFamilyProperties> &queueFamilies) {
  // Log the number of available queue families based on ENGINE_DEBUG
  ENGINE_DEBUG("There are {} queue families available on the system.", queueFamilies.size());

  for (uint32_t i = 0; i < queueFamilies.size(); ++i) {
    vk::QueueFamilyProperties queueFamily = queueFamilies[i];

    ENGINE_DEBUG("Queue Family {}:", i);

    std::string supportedQueues = "\tSupports ";

    bool anyFlags = false;
    if (queueFamily.queueFlags & vk::QueueFlagBits::eCompute) {
      supportedQueues += "compute, ";
      anyFlags = true;
    }
    if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
      supportedQueues += "graphics, ";
      anyFlags = true;
    }
    if (queueFamily.queueFlags & vk::QueueFlagBits::eTransfer) {
      supportedQueues += "transfer, ";
      anyFlags = true;
    }
    if (queueFamily.queueFlags & vk::QueueFlagBits::eOpticalFlowNV) {
      supportedQueues += "nvidia optical flow, ";
      anyFlags = true;
    }
    if (queueFamily.queueFlags & vk::QueueFlagBits::eSparseBinding) {
      supportedQueues += "sparse binding, ";
      anyFlags = true;
    }
    if (queueFamily.queueFlags & vk::QueueFlagBits::eProtected) {
      supportedQueues += "protected memory, ";
      anyFlags = true;
    }
    if (queueFamily.queueFlags & vk::QueueFlagBits::eVideoDecodeKHR) {
      supportedQueues += "video decode, ";
      anyFlags = true;
    }
    if (queueFamily.queueFlags & vk::QueueFlagBits::eVideoEncodeKHR) {
      supportedQueues += "video encode, ";
      anyFlags = true;
    }

    // Remove the last comma and space if any flags were added
    if (anyFlags) {
      supportedQueues = supportedQueues.substr(0, supportedQueues.size() - 2);
    }
    supportedQueues += ".";

    ENGINE_DEBUG("{}", supportedQueues);

    ENGINE_DEBUG("\tFamily supports {} queues.", queueFamily.queueCount);
  }
}
