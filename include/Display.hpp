#pragma once

# include "opengl/my_opengl.hpp"
# include "my_glfw.hpp"

class Logic;

class Display
{
  GLFWwindow *window;

public:

  Display(GLFWwindow&);
  Display(Display const &) = delete;
  Display(Display &&) = delete;

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
