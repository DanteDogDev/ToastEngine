#pragma once
#include "device.hpp"
using namespace Engine::Vulkan;

vk::ShaderModule MakeShaderModule(DeviceManager &devices, const std::vector<char> &code);

/**
 * @brief Create a shader object
 *    requires VK_EXT_shader_object device extension
 * @param logicalDevice vulkan device
 * @param stage shader stage for the module
 * @param filename name of the file holding the code
 * @return vk::ShaderEXT The created shader object
 */
std::vector<vk::ShaderEXT> MakeShaderObjects(DeviceManager &devices, vk::DispatchLoaderDynamic &dl,    //
                                             const char *vertexFilname, const char *fragmentFilename); //
