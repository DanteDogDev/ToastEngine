#include "renderer.hpp"
#include "device.hpp"
#include "instance.hpp"
#include "src/core/log.hpp"
namespace Engine::Vulkan {
Renderer::Renderer() {
  ENGINE_INFO("Making Renderer");
  m_instance = MakeInstance("Vulkat Engine", m_instanceDeletionQueue);
  // m_dldi = vk::DispatchLoaderDynamic(m_instance, vkGetInstanceProcAddr);
  m_dldi.init(m_instance, vkGetInstanceProcAddr);
  m_debugMessenger = MakeDebugMessenger(m_instance, m_dldi, m_instanceDeletionQueue);
  m_physicalDevice = ChoosePhysicalDevice(m_instance);
}

Renderer::~Renderer() {
  while (m_deviceDeletionQueue.size() > 0) {
    m_deviceDeletionQueue.back()(m_logicalDevice);
    m_deviceDeletionQueue.pop_back();
  }
  while (m_instanceDeletionQueue.size() > 0) {
    m_instanceDeletionQueue.back()(m_instance);
    m_instanceDeletionQueue.pop_back();
  }
}
void Renderer::Init() {}

} // namespace Engine::Vulkan
