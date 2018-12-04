#include "Display.hpp"

#include "Logic.hpp"
#include "GameState.hpp"
#include "TutoState.hpp"
#include "GameOverState.hpp"

#include <thread>
#include <cassert>

Logic::Logic()
  : running(true)
  , state(new state::TutoState())
{}

std::string Logic::getTime(void) const
{
  auto secondTime((time * getTickTime().count()) / 1000000);
  std::string   toReturn;

  if (secondTime / 60 >= 10)
    toReturn = std::to_string(secondTime / 60) + " m ";
  else if (secondTime / 60)
    toReturn = "0" + std::to_string(secondTime / 60) + " m ";
  if ((secondTime) % 60 >= 10)
    toReturn += std::to_string((secondTime) % 60) + " s";
  else
    toReturn += "0" + std::to_string((secondTime) % 60) + " s";
  return (toReturn);
}

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
    time++;
    state::StateType type = state->update(time);
    switch (type)
      {
        case state::GAME_STATE:
	  time = 0;
	  timeSinceStateStart = 0;
          state.reset(new state::GameState());
          break;
        case state::GAME_OVER_STATE:
	  timeSinceStateStart = 0;
          state.reset(new state::GameOverState(time, lastDispData));
          break;
        case state::WIN_STATE:
	  timeSinceStateStart = 0;
	  state.reset(new state::GameOverState(time, lastDispData, true));
          break;
        case state::BREAK:
          running = false;
          break;
        case state::CONTINUE:
	  timeSinceStateStart++;
	  break;
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
  if (timeSinceStateStart > eventDelay)
    state->handleKey(window, key);
}

void Logic::handleMouse(input::Input const &disp, GLFWwindow *win, input::Mouse mouse)
{
  //handle mouse moving here
  if (timeSinceStateStart > eventDelay)
    state->handleMouse(disp, win, mouse);
}

void Logic::handleButton(GLFWwindow *win, input::Button button)
{
  //handle mouse button here
  if (timeSinceStateStart > eventDelay)
    state->handleButton(win, button);
}

void Logic::checkEvents(input::Input &input)
{
  //check events there
  if (timeSinceStateStart > eventDelay)
    state->checkEvents(input);
}

void Logic::getObjectsToRender(DisplayData &displayData)
{
  state->getObjectsToRender(displayData);
  displayData.stringedTime = getTime();
  lastDispData = displayData;
}
