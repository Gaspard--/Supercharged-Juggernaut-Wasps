#pragma once

# include "State.hpp"
# include "Entity.hpp"

namespace state
{

  class GameState: public State
  {

  public:
    StateType update() override;
    void handleKey(GLFWwindow *window, input::Key key) override;
    void handleMouse(Display const &, GLFWwindow *window, input::Mouse mouse) override;
    void handleButton(GLFWwindow *window, input::Button button) override;
    void checkEvents(input::Input& input) override;
  };

}
