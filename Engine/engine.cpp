#include "src/core/window.hpp"
#include "src/vulkan/InstanceManager.hpp"
#include <cstdlib>  /*EXIT_FAILURE, EXIT_SUCCESS*/
#include <iostream> /*in/out*/

int test() {
  Engine::Core::Window window(800, 600, "Vulkat Engine");
  window.Init();
  try {
    Engine::Renderer::InstanceManager instance;
    while (!window.ShouldClose()) {
      window.Tick();
    }
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
  }
  window.Close();
  return EXIT_SUCCESS;
}
