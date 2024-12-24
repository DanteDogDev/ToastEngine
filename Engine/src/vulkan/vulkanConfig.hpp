namespace VulkanConfig {
static std::vector<const char *> instanceExtensions = {
#ifdef DEBUG
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME, // REMOVE THESE IF NOT DEBUG
#endif
};
static std::vector<const char *> instanceLayers = {
#ifdef DEBUG
    "VK_LAYER_KHRONOS_validation", // REMOVE THESE IF NOT DEBUG
#endif
};
static std::vector<const char *> physicalDeviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    "VK_EXT_shader_object",

};

static std::vector<vk::DynamicState> pipelineDynamicStates = {
    vk::DynamicState::eViewport,
    vk::DynamicState::eScissor,
};

} // namespace VulkanConfig
