#include "vklog.hpp"

void LogList(const char **list, u32 size) {
  for (u32 i = 0; i < size; i++) {
    ENGINE_DEBUG("\t\"{}\"", list[i]);
  }
}
void LogList(const std::vector<const char *> &list) {
  for (const char *token : list) {
    ENGINE_DEBUG("\t{}", token);
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

void LogVersion(u32 version) {
  ENGINE_DEBUG(                                                                 //
      "System can support Vulkan Variant: {}, Major: {}, Minor: {}, Patch: {}", //
      vk::apiVersionVariant(version),                                           //
      vk::apiVersionMajor(version),                                             //
      vk::apiVersionMinor(version),                                             //
      vk::apiVersionPatch(version)                                              //
  );
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

void LogList(const std::vector<vk::PresentModeKHR> &modes) {
  ENGINE_DEBUG("Supported Present Modes");
  for (vk::PresentModeKHR mode : modes) {
    ENGINE_DEBUG("\t{}", vk::to_string(mode));
  }
}

void LogList(const std::vector<vk::SurfaceFormatKHR> &formats) {
  for (vk::SurfaceFormatKHR format : formats) {
    ENGINE_DEBUG("Supported Pixel Format: {}, supported color space: {}", vk::to_string(format.format), vk::to_string(format.colorSpace));
  }
}

void Log(const vk::Extent2D &extent) {
  ENGINE_DEBUG("\t\twidth: {}", extent.width);
  ENGINE_DEBUG("\t\twidth: {}", extent.height);
}

void LogImageUsageBits(vk::ImageUsageFlags bits) {
  if (bits & vk::ImageUsageFlagBits::eTransferSrc) {
    ENGINE_DEBUG("transfer src: image can be used as the source of a transfer command.");
  }
  if (bits & vk::ImageUsageFlagBits::eTransferDst) {
    ENGINE_DEBUG("transfer dst: image can be used as the destination of a transfer command.");
  }
  if (bits & vk::ImageUsageFlagBits::eSampled) {
    ENGINE_DEBUG("sampled: image can be used to create a VkImageView suitable for occupying a \
VkDescriptorSet slot either of type VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE or \
VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, and be sampled by a shader.");
  }
  if (bits & vk::ImageUsageFlagBits::eStorage) {
    ENGINE_DEBUG("storage: image can be used to create a VkImageView suitable for occupying a \
VkDescriptorSet slot of type VK_DESCRIPTOR_TYPE_STORAGE_IMAGE.");
  }
  if (bits & vk::ImageUsageFlagBits::eColorAttachment) {
    ENGINE_DEBUG("color attachment: image can be used to create a VkImageView suitable for use as \
a color or resolve attachment in a VkFramebuffer.");
  }
  if (bits & vk::ImageUsageFlagBits::eDepthStencilAttachment) {
    ENGINE_DEBUG("depth/stencil attachment: image can be used to create a VkImageView \
suitable for use as a depth/stencil or depth/stencil resolve attachment in a VkFramebuffer.");
  }
  if (bits & vk::ImageUsageFlagBits::eTransientAttachment) {
    ENGINE_DEBUG("transient attachment: implementations may support using memory allocations \
with the VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT to back an image with this usage. This \
bit can be set for any image that can be used to create a VkImageView suitable for use as \
a color, resolve, depth/stencil, or input attachment.");
  }
  if (bits & vk::ImageUsageFlagBits::eInputAttachment) {
    ENGINE_DEBUG("input attachment: image can be used to create a VkImageView suitable for \
occupying VkDescriptorSet slot of type VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT; be read from \
a shader as an input attachment; and be used as an input attachment in a framebuffer.");
  }
  if (bits & vk::ImageUsageFlagBits::eFragmentDensityMapEXT) {
    ENGINE_DEBUG("fragment density map: image can be used to create a VkImageView suitable \
for use as a fragment density map image.");
  }
  if (bits & vk::ImageUsageFlagBits::eFragmentShadingRateAttachmentKHR) {
    ENGINE_DEBUG("fragment shading rate attachment: image can be used to create a VkImageView \
suitable for use as a fragment shading rate attachment or shading rate image");
  }
}

void LogAlphaCompositeBits(vk::CompositeAlphaFlagsKHR bits) {
  if (bits & vk::CompositeAlphaFlagBitsKHR::eOpaque) {
    ENGINE_DEBUG("		opaque (alpha ignored)");
  }
  if (bits & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied) {
    ENGINE_DEBUG("		pre multiplied (alpha expected to already be multiplied in image)");
  }
  if (bits & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied) {
    ENGINE_DEBUG("		post multiplied (alpha will be applied during composition)");
  }
  if (bits & vk::CompositeAlphaFlagBitsKHR::eInherit) {
    ENGINE_DEBUG("		inherited");
  }
}

void LogTransformBits(vk::SurfaceTransformFlagsKHR bits) {
  if (bits & vk::SurfaceTransformFlagBitsKHR::eIdentity) {
    ENGINE_DEBUG("		identity");
  }
  if (bits & vk::SurfaceTransformFlagBitsKHR::eRotate90) {
    ENGINE_DEBUG("		90 degree rotation");
  }
  if (bits & vk::SurfaceTransformFlagBitsKHR::eRotate180) {
    ENGINE_DEBUG("		180 degree rotation");
  }
  if (bits & vk::SurfaceTransformFlagBitsKHR::eRotate270) {
    ENGINE_DEBUG("		270 degree rotation");
  }
  if (bits & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirror) {
    ENGINE_DEBUG("		horizontal mirror");
  }
  if (bits & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate90) {
    ENGINE_DEBUG("		horizontal mirror, then 90 degree rotation");
  }
  if (bits & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate180) {
    ENGINE_DEBUG("		horizontal mirror, then 180 degree rotation");
  }
  if (bits & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate270) {
    ENGINE_DEBUG("		horizontal mirror, then 270 degree rotation");
  }
  if (bits & vk::SurfaceTransformFlagBitsKHR::eInherit) {
    ENGINE_DEBUG("		inherited");
  }
}

void LogList(const vk::SurfaceCapabilitiesKHR &capabilities) {
  ENGINE_DEBUG("Swapchain can support the following surface capabilities");
  ENGINE_DEBUG("\tminimum image count: {}", capabilities.minImageCount);
  ENGINE_DEBUG("\tmaximum image count: {}", capabilities.maxImageCount);
  ENGINE_DEBUG("\tcurrent extent: ");
  Log(capabilities.currentExtent);
  ENGINE_DEBUG("\tminimum supported extent: ");
  Log(capabilities.minImageExtent);
  ENGINE_DEBUG("\tmaximum supported extent: ");
  Log(capabilities.maxImageExtent);
  ENGINE_DEBUG("\tmaximum image array layers: {}", capabilities.maxImageArrayLayers);
  ENGINE_DEBUG("\tsupported transforms: ");
  LogTransformBits(capabilities.supportedTransforms);
  ENGINE_DEBUG("\tcurrent transform: ");
  LogTransformBits(capabilities.currentTransform);
  ENGINE_DEBUG("\tsupported alpha operations: ");
  LogAlphaCompositeBits(capabilities.supportedCompositeAlpha);
  ENGINE_DEBUG("\tsupported image usage: ");
  LogImageUsageBits(capabilities.supportedUsageFlags);
}
