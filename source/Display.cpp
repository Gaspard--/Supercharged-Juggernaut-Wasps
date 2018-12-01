# include "Display.hpp"
# include "Logic.hpp"

Display::Display(GLFWwindow &window)
  : window(&window)
{}

void Display::render()
{
  //do final render here
}

void Display::copyRenderData(Logic const &)
{
  //fetch all display informations from logic here
}
