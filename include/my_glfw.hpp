#pragma once

#include <GLFW/glfw3.h>
#include <memory>
#include <array>
#include <string>

// #include "util/Vect.hpp"

struct GlfwContext
{
  GlfwContext()
  {
    glfwSetErrorCallback([](int, char const *str) { throw std::runtime_error(str); });
    if (!glfwInit())
      throw std::runtime_error("my_glfw: failed to initialize glfw");
  }

  GlfwContext(GlfwContext const &) = delete;
  GlfwContext(GlfwContext &&) = delete;

  ~GlfwContext()
  {
    glfwTerminate();
  }

  std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> createWindow(std::array<unsigned int, 2u> dim, std::string name) const
  {
    std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)>
      window(glfwCreateWindow(static_cast<int>(dim[0]), static_cast<int>(dim[1]), name.c_str(), nullptr, nullptr), &glfwDestroyWindow);

    if (!window)
      throw std::runtime_error("my_glfw: failed to open window");
    glfwMakeContextCurrent(window.get());
    glfwSwapInterval(1);
    return window;
  }
};
