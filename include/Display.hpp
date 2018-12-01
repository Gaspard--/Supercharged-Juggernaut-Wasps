#pragma once

# include "my_glfw.hpp"

class Logic;

class Display
{
  GLFWwindow *window;

public:

  Display(GLFWwindow&);

  void render();
  void copyRenderData(Logic const&);

  GLFWwindow *getWindow() const
  {
    return window;
  }

  bool isRunning() const
  {
    return (!glfwWindowShouldClose(window));
  }
};
