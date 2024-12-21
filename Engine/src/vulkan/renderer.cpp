#include "renderer.hpp"
namespace Engine::Vulkan {
Renderer::Renderer(GLFWwindow *window) {
  m_instanceManager.Init();
  m_surface = m_instanceManager.MakeSurface(window);
  m_deviceManager.Init(m_instanceManager.m_instance, m_surface);
}

Renderer::~Renderer() {}

} // namespace Engine::Vulkan
