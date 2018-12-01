#include "Display.hpp"
#include "Logic.hpp"

#include <iostream>

Display::Display(GLFWwindow &window)
  : window(&window)
{
}

void Display::render()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  //do final render here
  glClearColor(0.15f, 0.08f, 0.11f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

void Display::copyRenderData(Logic const &)
{
  //fetch all display informations from logic here
}
