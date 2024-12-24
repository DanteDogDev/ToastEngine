#include "src/core/window.hpp"
#include "src/vulkan/renderer.hpp"
#include <atomic>
#include <thread>
using namespace Engine;
void Spawn_Render_Thread(GLFWwindow *window, std::atomic<bool> *done) {
  VulkanEngine *engine = new VulkanEngine(window);
  // make engine
  while (!*done) {
    engine->Draw();
    // delete engine
  }
  delete engine;
}

void Run() {
  Core::Log::Init();
  ENGINE_INFO("Init");
  Core::Window app(800, 600, "Vulkat Engine");
  // VulkanEngine renderer(app.m_window);

  std::atomic<bool> done = false;
  std::thread render_thread(Spawn_Render_Thread, app.m_window, &done);
  ENGINE_INFO("Run")
  app.Run();
  done = true;
  render_thread.join();
  ENGINE_INFO("Close");
  // spdlog::info("sad");
}
