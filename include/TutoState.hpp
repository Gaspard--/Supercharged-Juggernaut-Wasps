#pragma once

# include "State.hpp"
# include "DisplayData.hpp"
# include "Input.hpp"

namespace state
{
  class TutoState : public State
  {
    bool isSkipped{false};
  public:
    TutoState() = default;
    StateType update(unsigned int&) override;
    void handleKey(GLFWwindow *window, input::Key key) override;
    void handleMouse(input::Input const &, GLFWwindow *, input::Mouse) override;
    void handleButton(GLFWwindow *, input::Button) override;
    void checkEvents(input::Input&) override;
    void getObjectsToRender(DisplayData &display) override;
  };
}
