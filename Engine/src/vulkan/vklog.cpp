#include "vklog.hpp"
void LogList(const std::vector<const char *> &list) {
  for (const auto &item : list) {
    ENGINE_DEBUG("\t\"{}\"", item);
  }
}
void LogList(const char **list, u32 size) {
  for (u32 i = 0; i < size; i++) {
    ENGINE_DEBUG("\t\"{}\"", list[i]);
  }
}
void LogExtensions(std::vector<vk::ExtensionProperties> &extensions) {
  for (const vk::ExtensionProperties &extension : extensions) {
    ENGINE_DEBUG("\t\'{}\'", std::string(extension.extensionName.data()));
  }
}
void LogLayers(std::vector<vk::LayerProperties> &layers) {
  for (const vk::LayerProperties &layer : layers) {
    ENGINE_DEBUG("\t\'{}\'", std::string(layer.layerName.data()));
  }
}
void ReportVersionNumber(u32 version) {
  ENGINE_DEBUG(                                                                 //
      "System can support Vulkan Variant: {}, Major: {}, Minor: {}, Patch: {}", //
      vk::apiVersionVariant(version),                                           //
      vk::apiVersionMajor(version),                                             //
      vk::apiVersionMinor(version),                                             //
      vk::apiVersionPatch(version)                                              //
  );
}
