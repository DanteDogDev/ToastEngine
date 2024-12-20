#include "vulkanEngine.hpp"
#include "src/core/log.hpp"
#include "src/core/window.hpp"
#include "src/vulkan/renderer.hpp"
#include <atomic>
#include <thread>

void Spawn_Render_Thread(GLFWwindow *window, std::atomic<bool> *done) {
  // make engine
  while (!*done) {

    // delete engine
  }
}

void Run() {
  Engine::Core::Log::Init();
  ENGINE_INFO("Init");
  Engine::Vulkan::Renderer renderer;
  Engine::Core::Window app(800, 600, "Vulkat Engine");

  std::atomic<bool> done = false;
  std::thread render_thread(Spawn_Render_Thread, app.m_window, &done);
  ENGINE_INFO("Run")
  app.Run();
  done = true;
  render_thread.join();
  ENGINE_INFO("Close");
  // spdlog::info("sad");
}
