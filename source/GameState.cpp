# include "GameState.hpp"

namespace state
{
  StateType GameState::update()
  {
    return CONTINUE;
  }

  void GameState::handleKey(GLFWwindow *, input::Key)
  {

  }

  void GameState::handleMouse(Display const &, GLFWwindow *, input::Mouse)
  {

  }

  void GameState::handleButton(GLFWwindow *, input::Button)
  {

  }

  void GameState::checkEvents(input::Input&)
  {

  }
}
