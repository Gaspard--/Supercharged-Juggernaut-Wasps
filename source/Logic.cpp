# include "Logic.hpp"
# include "Display.hpp"

Logic::Logic(): running(true) {}

void Logic::tick(std::mutex &)
{
  //do logic
}

void Logic::handleEvent(Display const &display, input::Event const& event)
{
  //
  // @Kellen : this is ugly, help me to remove that pls
  //
  if (event)
    {
      switch (event.data.index())
        {
        case 1:
          handleKey(event.window, std::get<input::Key>(event.data));
          break;
        case 2:
          handleMouse(display, event.window, std::get<input::Mouse>(event.data));
          break;
        case 3:
          handleButton(event.window, std::get<input::Button>(event.data));
          break;
        default:
          break;
        }
    }
}

void Logic::handleKey(GLFWwindow *window, input::Key key)
{
  switch (key.key)
    {
    case GLFW_KEY_ESCAPE:
      glfwSetWindowShouldClose(window, true);
      break;
    default:
      break;
    }
}

void Logic::handleMouse(Display const &, GLFWwindow *, input::Mouse)
{
  //handle mouse moving here
}

void Logic::handleButton(GLFWwindow *, input::Button)
{
  //handle mouse button here
}

void Logic::checkEvents(input::Input&)
{
  //check events there
}
