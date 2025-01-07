#include "shader.hpp"
#include "src/common/file.hpp"
#include "src/common/log.hpp"
#include "src/vulkan/device.hpp"

vk::ShaderModule MakeShaderModule(DeviceManager &devices, const std::vector<char> &code) {
  vk::ShaderModuleCreateInfo createInfo{};
  createInfo.setCodeSize(code.size());
  createInfo.setPCode(reinterpret_cast<const u32 *>(code.data()));

  vk::ShaderModule shaderModule;
  try {
    shaderModule = devices.m_logicalDevice.createShaderModule(createInfo);
    devices.m_deviceDeletionQueue.push_back([shaderModule](vk::Device device) {
      ENGINE_DEBUG("Deleting Shader Module");
      device.destroyShaderModule(shaderModule, nullptr);
    });
  } catch (std::exception &e) {
    ENGINE_ERROR("Failed to create shader module: {}", e.what());
  }
  return shaderModule;
}
