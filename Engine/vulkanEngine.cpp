#include <exception>
#include <iostream> /*in/out*/

using namespace Engine;
VulkanEngine::VulkanEngine() : m_window(800, 600, "Vulkat Engine"), InstanceManager() {}
void VulkanEngine::Init() {
  std::cout << "Init\n";
  m_window.Init();
  InstanceManager.CreateInstance();
}
void VulkanEngine::Run() {
  std::cout << "Run\n";
  try {
    while (!m_window.ShouldClose()) {
      m_window.Tick();
    }
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
  }
  m_window.Close();
}
void VulkanEngine::Cleanup() { std::cout << "Cleanup\n"; }
