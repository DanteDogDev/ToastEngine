#include <string>
#define GLFW_INCLUDE_VULKAN /* changes GLFW to vulkan */
#include <GLFW/glfw3.h>
namespace Engine::Core {
class Window {
public:
  Window(int width, int height, std::string title);
  ~Window();

  void Init();
  void Tick();
  void Close();
  double DeltaTime();
  inline bool ShouldClose() const { return glfwWindowShouldClose(m_window); };

private:
  GLFWwindow *m_window;
  int m_width;
  int m_height;
  std::string m_title;
};
} // namespace Engine::Core
