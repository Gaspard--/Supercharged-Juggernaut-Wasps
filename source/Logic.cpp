# include "Logic.hpp"
# include "Display.hpp"
# include "GameState.hpp"

Logic::Logic()
  : running(true)
  , state(new state::GameState())
{}

void Logic::tick(std::mutex &)
{
  state::StateType type = state->update();
  switch (type)
    {
    case state::GAME_STATE:
      state.reset(new state::GameState());
      break;
    case state::BREAK:
      running = false;
      break;
    case state::CONTINUE:
    default:
      break;
    }
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
  state->handleKey(window, key);
}

void Logic::handleMouse(Display const &disp, GLFWwindow *win, input::Mouse mouse)
{
  //handle mouse moving here
  state->handleMouse(disp, win, mouse);
}

void Logic::handleButton(GLFWwindow *win, input::Button button)
{
  //handle mouse button here
  state->handleButton(win, button);
}

void Logic::checkEvents(input::Input &input)
{
  //check events there
  state->checkEvents(input);
}
