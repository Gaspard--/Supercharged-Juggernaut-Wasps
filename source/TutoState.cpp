# include "TutoState.hpp"

namespace state
{
  StateType TutoState::update(unsigned int& time)
  {
    if (isSkipped)
      return GAME_STATE;
    return CONTINUE;
  }

  void TutoState::handleKey(GLFWwindow *, input::Key)
  {
    isSkipped = true;
  }

  void TutoState::handleMouse(input::Input const &, GLFWwindow *, input::Mouse)
  {

  }

  void TutoState::handleButton(GLFWwindow *, input::Button)
  {
    isSkipped = true;
  }

  void TutoState::checkEvents(input::Input& input)
  {
    std::vector<unsigned char> buttons = input.getJoystickButtons();
    for (auto i = buttons.begin() ; i != buttons.end() ; ++i)
      if (*i == GLFW_PRESS) {
	isSkipped = true;
	return ;
      }
  }

  void TutoState::getObjectsToRender(DisplayData &display)
  {
    display.tuto = true;
  }
}
