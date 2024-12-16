#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN /* changes GLFW to vulkan */
#include <GLFW/glfw3.h>

#include "defines.h"
#include <cstdlib>   /*EXIT_FAILURE, EXIT_SUCCESS*/
#include <iostream>  /*in/out*/
#include <stdexcept> /*reporting errors */

class Engine {
public:
  void run() {
    std::cout << "initWindow\n";
    initWindow();
    std::cout << "initVulkan\n";
    initVulkan();
    std::cout << "mainLoop\n";
    mainLoop();
    std::cout << "cleanup\n";
    cleanup();
  }

private:
  VkInstance instance;
  GLFWwindow *window;
  void initWindow() {
    glfwInit();
    // Sets default api to none otherwise it would have been set to OPENGL
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // Sets the resizable flag to false
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(800, 600, "Toast Engine - Vulkan", nullptr, nullptr);
  }
  void initVulkan() { createInstance(); }

  void mainLoop() {
    while (!glfwWindowShouldClose(window)) { /*checks the close flag on the window */
      glfwPollEvents();                      /* checks for events on the window */
    }
  }

  void cleanup() {
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
  }
  void createInstance() {
    /*Technicly Optinal */
    VkApplicationInfo appInfo;
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = nullptr;
    appInfo.pApplicationName = "Test Toast";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Toast Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;
    /*Not Optinal*/
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    u32 glfwExtensionCount = 0;
    const char **glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = 0;
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
      throw std::runtime_error("failed to create instance!");
    }
  }
};

int test() {
  std::cout << "DLL Working\n";
  Engine app;
  try {
    app.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
