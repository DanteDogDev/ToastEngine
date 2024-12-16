#include <vulkanEngine.hpp>
using namespace Engine;
int main() {
  VulkanEngine engine;
  engine.Init();
  engine.Run();
  engine.Cleanup();
  return 0;
}
