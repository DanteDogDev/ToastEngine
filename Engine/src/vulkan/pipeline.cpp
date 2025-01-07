#include "src/vulkan/pipeline.hpp"
#include "shader.hpp"
#include "src/common/file.hpp"
#include "src/common/log.hpp"
#include "vulkanConfig.hpp"
using namespace Engine::Vulkan;

void PipelineManager::Init(DeviceManager &devices, Swapchain &swapchain) {
  ENGINE_DEBUG("Making Pipeline Manager");
  m_graphicsPipeline = MakeGraphicsPipeline(devices, swapchain);
}

vk::Pipeline PipelineManager::MakeGraphicsPipeline(DeviceManager &devices, Swapchain &swapchain) {
  ENGINE_INFO("Creating Graphics Pipeline");
  std::vector<char> vertShaderCode = ReadFile("bin/Resources/shaders/vertex.spv");
  std::vector<char> fragShaderCode = ReadFile("bin/Resources/shaders/fragment.spv");

  vk::ShaderModule vertexModule = MakeShaderModule(devices, vertShaderCode);
  vk::ShaderModule fragmentModule = MakeShaderModule(devices, fragShaderCode);

  // Shader Stage (replaced by shader object)
  vk::PipelineShaderStageCreateInfo vertShaderStageInfo{};
  vertShaderStageInfo.setStage(vk::ShaderStageFlagBits::eVertex);
  vertShaderStageInfo.setModule(vertexModule);
  vertShaderStageInfo.setPName("main");
  vk::PipelineShaderStageCreateInfo fragShaderStageInfo{};
  fragShaderStageInfo.setStage(vk::ShaderStageFlagBits::eFragment);
  fragShaderStageInfo.setModule(fragmentModule);
  fragShaderStageInfo.setPName("main");
  vk::PipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

  // Dynamic states
  vk::PipelineDynamicStateCreateInfo dynamicState{};
  dynamicState.setDynamicStateCount(VulkanConfig::pipelineDynamicStates.size());
  dynamicState.setPDynamicStates(VulkanConfig::pipelineDynamicStates.data());

  // Vertex Input
  vk::PipelineVertexInputStateCreateInfo vertexInputInfo{};
  vertexInputInfo.setVertexBindingDescriptionCount(0);
  vertexInputInfo.setVertexBindingDescriptions(nullptr);
  vertexInputInfo.setVertexAttributeDescriptionCount(0);
  vertexInputInfo.setVertexAttributeDescriptions(nullptr);

  // Input Assenbly
  vk::PipelineInputAssemblyStateCreateInfo inputAssembly{};
  inputAssembly.setTopology(vk::PrimitiveTopology::eTriangleList);
  inputAssembly.setPrimitiveRestartEnable(false);

  // viewport and scissors
  vk::Viewport viewport{};
  viewport.setX(0.0f);
  viewport.setY(0.0f);
  viewport.setWidth((float)swapchain.m_extent.width);
  viewport.setHeight((float)swapchain.m_extent.height);
  viewport.setMinDepth(0.0f);
  viewport.setMaxDepth(1.0f);
  vk::Rect2D scissor{};
  scissor.setOffset({0, 0});
  scissor.setExtent(swapchain.m_extent);

  vk::PipelineViewportStateCreateInfo viewportState{};
  viewportState.setViewportCount(1);
  viewportState.setPViewports(&viewport);
  viewportState.setScissorCount(1);
  viewportState.setPScissors(&scissor);

  // Rasterizer
  vk::PipelineRasterizationStateCreateInfo rasterizer{};
  rasterizer.setDepthClampEnable(false);
  rasterizer.setRasterizerDiscardEnable(false);
  rasterizer.setPolygonMode(vk::PolygonMode::eFill);
  rasterizer.setLineWidth(1.0f);
  rasterizer.setCullMode(vk::CullModeFlagBits::eBack);
  rasterizer.setFrontFace(vk::FrontFace::eClockwise);
  rasterizer.setDepthBiasEnable(false);
  rasterizer.setDepthBiasConstantFactor(0.0f);
  rasterizer.setDepthBiasClamp(0.0f);
  rasterizer.setDepthBiasSlopeFactor(0.0f);

  // Multisampling / anti-aliasing
  vk::PipelineMultisampleStateCreateInfo multisampling{};
  multisampling.setSampleShadingEnable(false);
  multisampling.setRasterizationSamples(vk::SampleCountFlagBits::e1);
  multisampling.setMinSampleShading(1.0f);
  multisampling.setPSampleMask(nullptr);
  multisampling.setAlphaToCoverageEnable(false);
  multisampling.setAlphaToOneEnable(false);

  // Depth and Stencil Testing
  vk::PipelineDepthStencilStateCreateInfo depthStencil{};
  depthStencil.setDepthTestEnable(true);
  depthStencil.setDepthWriteEnable(true);
  depthStencil.setDepthCompareOp(vk::CompareOp::eLess);
  depthStencil.setDepthBoundsTestEnable(false);
  depthStencil.setStencilTestEnable(false);

  // Color Blending
  vk::PipelineColorBlendAttachmentState colorBlendAttachment{};
  colorBlendAttachment.setColorWriteMask(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | //
                                         vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA); //
  colorBlendAttachment.setBlendEnable(false);
  colorBlendAttachment.setSrcColorBlendFactor(vk::BlendFactor::eSrcAlpha);
  colorBlendAttachment.setDstColorBlendFactor(vk::BlendFactor::eOneMinusSrcAlpha);
  colorBlendAttachment.setColorBlendOp(vk::BlendOp::eAdd);
  colorBlendAttachment.setSrcAlphaBlendFactor(vk::BlendFactor::eOne);
  colorBlendAttachment.setDstAlphaBlendFactor(vk::BlendFactor::eZero);
  colorBlendAttachment.setAlphaBlendOp(vk::BlendOp::eAdd);
  vk::PipelineColorBlendStateCreateInfo colorBlending{};
  colorBlending.setLogicOpEnable(false);
  colorBlending.setLogicOp(vk::LogicOp::eCopy);
  colorBlending.setAttachmentCount(1);
  colorBlending.setPAttachments(&colorBlendAttachment);
  colorBlending.blendConstants[0] = 0.0f;
  colorBlending.blendConstants[1] = 0.0f;
  colorBlending.blendConstants[2] = 0.0f;
  colorBlending.blendConstants[3] = 0.0f;

  // Pipeline layout
  vk::PipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.setSetLayoutCount(0);
  pipelineLayoutInfo.setPSetLayouts(nullptr);
  pipelineLayoutInfo.setPushConstantRangeCount(0);
  pipelineLayoutInfo.setPPushConstantRanges(nullptr);

  m_renderPass = MakeRenderPass(devices, swapchain);
  vk::PipelineLayout pipelineLayout;

  try {
    pipelineLayout = devices.m_logicalDevice.createPipelineLayout(pipelineLayoutInfo);
    devices.m_deviceDeletionQueue.push_back([pipelineLayout](vk::Device device) {
      ENGINE_DEBUG("Destroying Pipeline layout");
      device.destroyPipelineLayout(pipelineLayout);
    });
  } catch (std::exception &e) {
    ENGINE_ERROR("Failed to create pipeline layout");
  }

  vk::GraphicsPipelineCreateInfo pipelineInfo{};
  pipelineInfo.setStageCount(2);
  pipelineInfo.setStages(shaderStages);
  pipelineInfo.setPVertexInputState(&vertexInputInfo);
  pipelineInfo.setPInputAssemblyState(&inputAssembly);
  pipelineInfo.setPViewportState(&viewportState);
  pipelineInfo.setPRasterizationState(&rasterizer);
  pipelineInfo.setPMultisampleState(&multisampling);
  pipelineInfo.setPDepthStencilState(&depthStencil);
  pipelineInfo.setPColorBlendState(&colorBlending);
  pipelineInfo.setPDynamicState(&dynamicState);

  pipelineInfo.setLayout(pipelineLayout);
  pipelineInfo.setRenderPass(m_renderPass);
  pipelineInfo.setSubpass(0);

  pipelineInfo.setBasePipelineHandle(nullptr);
  pipelineInfo.setBasePipelineIndex(-1);

  vk::Pipeline graphicsPipeline;

  try {
    graphicsPipeline = devices.m_logicalDevice.createGraphicsPipeline(nullptr, pipelineInfo).value;
    devices.m_deviceDeletionQueue.push_back([graphicsPipeline](vk::Device device) {
      ENGINE_DEBUG("Deleting Graphics Pipeline");
      device.destroyPipeline(graphicsPipeline);
    });
  } catch (std::exception &e) {
    ENGINE_ERROR("Failed to create graphics pipeline");
  }
  return graphicsPipeline;
}

vk::RenderPass PipelineManager::MakeRenderPass(DeviceManager &devices, Swapchain &swapchain) {
  vk::AttachmentDescription colorAttachment{};
  colorAttachment.setFormat(swapchain.m_format.format);
  colorAttachment.setSamples(vk::SampleCountFlagBits::e1);
  colorAttachment.setLoadOp(vk::AttachmentLoadOp::eClear);
  colorAttachment.setStoreOp(vk::AttachmentStoreOp::eStore);
  colorAttachment.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare);
  colorAttachment.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare);
  colorAttachment.setInitialLayout(vk::ImageLayout::eUndefined);
  colorAttachment.setFinalLayout(vk::ImageLayout::ePresentSrcKHR);

  vk::AttachmentReference colorAttachmentRef{};
  colorAttachmentRef.setAttachment(0);
  colorAttachmentRef.setLayout(vk::ImageLayout::eColorAttachmentOptimal);

  vk::SubpassDescription subpass{};
  subpass.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics);
  subpass.setColorAttachmentCount(1);
  subpass.setPColorAttachments(&colorAttachmentRef);

  vk::SubpassDependency dependency{};
  dependency.setSrcSubpass(VK_SUBPASS_EXTERNAL);
  dependency.setDstSubpass(0);
  dependency.setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
  dependency.setSrcAccessMask(vk::AccessFlagBits::eNone);
  dependency.setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
  dependency.setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite);

  vk::RenderPassCreateInfo renderPassInfo{};
  renderPassInfo.setAttachmentCount(1);
  renderPassInfo.setPAttachments(&colorAttachment);
  renderPassInfo.setSubpassCount(1);
  renderPassInfo.setPSubpasses(&subpass);
  renderPassInfo.setDependencyCount(1);
  renderPassInfo.setPDependencies(&dependency);

  vk::RenderPass renderPass;
  try {
    renderPass = devices.m_logicalDevice.createRenderPass(renderPassInfo);
    devices.m_deviceDeletionQueue.push_back([renderPass](vk::Device device) {
      ENGINE_DEBUG("Deleting RenderPass");
      device.destroyRenderPass(renderPass);
    });
  } catch (std::exception &e) {
    ENGINE_ERROR("Failed to create RenderPass")
  }
  return renderPass;
}
