#pragma once

# include "Display.hpp"
# include "Input.hpp"

namespace state
{

  enum StateType
    {
     CONTINUE,   // continue with current state
     GAME_STATE, // change for game_state
     GAME_OVER_STATE, // change for game_over_state
     WIN_STATE, // change for game_over_state, but win
     BREAK       // quit
    };

  class State
  {

  public:
    virtual ~State() = default;
    virtual StateType update(unsigned int &time) = 0;
    virtual void handleKey(GLFWwindow *window, input::Key key) = 0;
    virtual void handleMouse(input::Input const &, GLFWwindow *window, input::Mouse mouse) = 0;
    virtual void handleButton(GLFWwindow *window, input::Button button) = 0;
    virtual void checkEvents(input::Input& input) = 0;
    virtual void getObjectsToRender(DisplayData &displayData) = 0;
  };

}
