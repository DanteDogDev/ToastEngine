#include "shader.hpp"
#include "src/common/file.hpp"
#include "src/common/log.hpp"
#include "src/vulkan/device.hpp"
#include <exception>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_structs.hpp>

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

std::vector<vk::ShaderEXT> MakeShaderObjects(DeviceManager &devices, vk::DispatchLoaderDynamic &dl, //
                                             const char *vertexFilname, const char *fragmentFilename) {
  ENGINE_INFO("Making Shader Objects")
  vk::ShaderCreateFlagsEXT flags = vk::ShaderCreateFlagBitsEXT::eLinkStage;
  vk::ShaderStageFlags nextStage = vk::ShaderStageFlagBits::eFragment;

  std::vector<char> vertexSrc = ReadFile(vertexFilname);
  vk::ShaderCodeTypeEXT codeType = vk::ShaderCodeTypeEXT::eSpirv;

  const char *name = "main";
  vk::ShaderCreateInfoEXT vertexInfo{};
  vertexInfo.setFlags(flags);
  vertexInfo.setStage(vk::ShaderStageFlagBits::eVertex);
  vertexInfo.setNextStage(nextStage);
  vertexInfo.setCodeType(codeType);
  vertexInfo.setCodeSize(vertexSrc.size());
  vertexInfo.setPCode(vertexSrc.data());
  vertexInfo.setPName(name);

  std::vector<char> fragmentSrc = ReadFile(fragmentFilename);

  vk::ShaderCreateInfoEXT fragmentInfo{};
  fragmentInfo.setFlags(flags);
  fragmentInfo.setStage(vk::ShaderStageFlagBits::eFragment);
  fragmentInfo.setCodeType(codeType);
  fragmentInfo.setCodeSize(fragmentSrc.size());
  fragmentInfo.setPCode(fragmentSrc.data());
  fragmentInfo.setPName(name);

  std::vector<vk::ShaderCreateInfoEXT> shaderInfo(2);
  shaderInfo[0] = vertexInfo;
  shaderInfo[1] = fragmentInfo;
  std::vector<vk::ShaderEXT> shaders;
  try {
    shaders = devices.m_logicalDevice.createShadersEXT(shaderInfo, nullptr, dl).value;
    ENGINE_DEBUG("Finished Making Shaders");
    VkShaderEXT vertex = shaders[0];
    VkShaderEXT fragment = shaders[1];
    devices.m_deviceDeletionQueue.push_back([vertex, dl](vk::Device device) {
      ENGINE_DEBUG("Destroying Vertex Shader");
      device.destroyShaderEXT(vertex, nullptr, dl);
    });
    devices.m_deviceDeletionQueue.push_back([fragment, dl](vk::Device device) {
      ENGINE_DEBUG("Destroying Fragment Shader");
      device.destroyShaderEXT(fragment, nullptr, dl);
    });
  } catch (std::exception &e) {
    ENGINE_CRITICAL("Could not create Shaders:{}", e.what());
  }
  return shaders;
}
