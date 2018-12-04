# include "GameOverState.hpp"

namespace state
{
  GameOverState::GameOverState(unsigned int time, const DisplayData& dispData, bool win)
    : frozenTime(time)
    , frozenDisplayData(dispData)
    , win(win)
  {}

  StateType GameOverState::update(unsigned int& time)
  {
    time = frozenTime;
    if (isSkipped)
      return GAME_STATE;
    return CONTINUE;
  }

  void GameOverState::handleKey(GLFWwindow *, input::Key)
  {
    isSkipped = true;
  }

  void GameOverState::handleMouse(input::Input const &, GLFWwindow *, input::Mouse)
  {

  }

  void GameOverState::handleButton(GLFWwindow *, input::Button)
  {
    isSkipped = true;
  }

  void GameOverState::checkEvents(input::Input& input)
  {
    std::vector<unsigned char> buttons = input.getJoystickButtons();
    for (auto i = buttons.begin() ; i != buttons.end() ; ++i)
      if (*i == GLFW_PRESS) {
	isSkipped = true;
	return ;
      }
  }

  void GameOverState::getObjectsToRender(DisplayData &display)
  {
    display = frozenDisplayData;
    display.gameOverHud = true;
    display.win = win;
  }
}
