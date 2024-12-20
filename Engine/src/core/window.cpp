#include "window.hpp"
#include "log.hpp"
namespace Engine::Core {

Window::Window(int width, int height, std::string title) : m_width(width), m_height(height), m_title(title) {
  ENGINE_DEBUG("Window Init");
  glfwInit();
  // Sets default api to none otherwise it would have been set to OPENGL
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  // Sets the resizable flag to false
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
}
Window::~Window() {
  ENGINE_DEBUG("Window Close");
  glfwDestroyWindow(m_window);
  glfwTerminate();
}

void Window::Init() {}

void Window::Tick() { glfwPollEvents(); }
void Window::Run() {
  try {
    while (!ShouldClose()) {
      Tick();
    }
  } catch (const std::exception &e) {
    ENGINE_ERROR("Exception caught in Run(): {}", e.what());
    Close();
    throw;
  }
  Close();
}
void Window::Close() { glfwSetWindowShouldClose(m_window, false); }
double Window::DeltaTime() { return 0.0; } // TODO: Implement Later
} // namespace Engine::Core
