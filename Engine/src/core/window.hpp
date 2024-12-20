#pragma once
namespace Engine::Core {
class Window {
public:
  Window(int width, int height, std::string title);
  ~Window();
  GLFWwindow *m_window;

  void Init();
  void Tick();
  void Run();
  void Close();
  double DeltaTime();
  inline bool ShouldClose() const { return glfwWindowShouldClose(m_window); };

private:
  int m_width;
  int m_height;
  std::string m_title;
};
} // namespace Engine::Core
