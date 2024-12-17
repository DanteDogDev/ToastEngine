#include <atomic>
#include <iostream> /*in/out*/
#include <thread>

void Spawn_Render_Thread(GLFWwindow *window, std::atomic<bool> *done) {
  // make engine
  while (!*done) {

    // delete engine
  }
}
using namespace Engine;
VulkanEngine::VulkanEngine() : m_app(800, 600, "Vulkat Engine"), m_instanceManager() {}
void VulkanEngine::Init() {
  std::cout << "Init\n";
  m_app.Init();
  m_instanceManager.Init();
}
void VulkanEngine::Run() {
  std::cout << "Run\n";

  std::atomic<bool> done = false;
  std::thread render_thread(Spawn_Render_Thread, m_app.m_window, &done);
  m_app.Run();
  done = true;
  render_thread.join();
}
void VulkanEngine::Cleanup() { std::cout << "Cleanup\n"; }
