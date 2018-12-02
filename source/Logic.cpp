#include "Display.hpp"

#include "Logic.hpp"
#include "GameState.hpp"

#include <thread>
#include <cassert>

Logic::Logic()
  : running(true)
  , state(new state::GameState())
{}

void Logic::tick(std::mutex &lock)
{
  auto const now(Clock::now());

  if (now > lastUpdate + getTickTime() * 2)
    {
      lastUpdate = now;
      return ;
    }
  lastUpdate += getTickTime();
  if (now < lastUpdate)
    std::this_thread::sleep_for(lastUpdate - now);

  {
    std::lock_guard<std::mutex> scopedLock(lock);
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
}

void Logic::handleEvent(input::Input const &, input::Event const& event, input::Key const &key)
{
  handleKey(event.window, key);
}

void Logic::handleEvent(input::Input const &input, input::Event const& event, input::Mouse const &mouse)
{
  handleMouse(input, event.window, mouse);
}

void Logic::handleEvent(input::Input const &, input::Event const& event, input::Button const &button)
{
  handleButton(event.window, button);
}

void Logic::handleEvent(input::Input const &, input::Event const&, input::None const &)
{
  assert(!"A non-event was passed to logic: this should never happen.");
}

void Logic::handleEvent(input::Input const &input, input::Event const& event)
{
  std::visit([&](auto const &data) noexcept
	     {
	       handleEvent(input, event, data);
	     }, event.data);
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

void Logic::handleMouse(input::Input const &disp, GLFWwindow *win, input::Mouse mouse)
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

void Logic::getObjectsToRender(DisplayData &displayData)
{
  state->getObjectsToRender(displayData);
}
