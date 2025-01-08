namespace VulkanConfig {
static std::vector<const char *> instanceExtensions = {
#ifdef DEBUG
    vk::EXTDebugUtilsExtensionName, // REMOVE THESE IF NOT DEBUG
#endif
};
static std::vector<const char *> instanceLayers = {
#ifdef DEBUG
    "VK_LAYER_KHRONOS_validation", // REMOVE THESE IF NOT DEBUG
#endif
};
static std::vector<const char *> physicalDeviceExtensions = {
    vk::KHRSwapchainExtensionName,

};

static std::vector<vk::DynamicState> pipelineDynamicStates = {
    vk::DynamicState::eViewport,
    vk::DynamicState::eScissor,
};

} // namespace VulkanConfig
