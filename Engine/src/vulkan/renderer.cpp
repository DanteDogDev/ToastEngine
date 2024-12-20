#include "renderer.hpp"
#include "instance.hpp"
#include "src/core/log.hpp"
namespace Engine::Vulkan {
Renderer::Renderer() {
  ENGINE_INFO("Making Renderer");
  m_instance = MakeInstance("Vulkat Engine", m_deletionQueue);
  // m_dldi = vk::DispatchLoaderDynamic(m_instance, vkGetInstanceProcAddr);
  m_dldi.init(m_instance, vkGetInstanceProcAddr);
  m_debugMessenger = MakeDebugMessenger(m_instance, m_dldi, m_deletionQueue);
}
Renderer::~Renderer() {
  while (m_deletionQueue.size() > 0) {
    m_deletionQueue.back()(m_instance);
    m_deletionQueue.pop_back();
  }
}
void Renderer::Init() {}

} // namespace Engine::Vulkan
