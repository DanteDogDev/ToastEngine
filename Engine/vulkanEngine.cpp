#include "vulkanEngine.hpp"
#include "src/core/window.hpp"
#include "src/vulkan/renderer.hpp"
#include <atomic>
#include <spdlog/spdlog.h>
#include <thread>

void Spawn_Render_Thread(GLFWwindow *window, std::atomic<bool> *done) {
  // make engine
  while (!*done) {

    // delete engine
  }
}

void Run() {
  spdlog::info("Init");
  Engine::Core::Window app(800, 600, "Vulkat Engine");
  Engine::Vulkan::Renderer renderer;

  std::atomic<bool> done = false;
  std::thread render_thread(Spawn_Render_Thread, app.m_window, &done);
  spdlog::info("Run");
  app.Run();
  done = true;
  render_thread.join();
  spdlog::info("Close");
}
