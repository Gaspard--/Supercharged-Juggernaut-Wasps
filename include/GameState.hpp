#pragma once

#include <optional>

#include "State.hpp"
#include "Wasp.hpp"

namespace state
{
  class GameState : public State
  {
    BigWasp bigWasp;
    std::optional<SmolWasp> smolWasp;
  public:
    GameState();
    StateType update() override;
    void handleKey(GLFWwindow *window, input::Key key) override;
    void handleMouse(Display const &, GLFWwindow *window, input::Mouse mouse) override;
    void handleButton(GLFWwindow *window, input::Button button) override;
    void checkEvents(input::Input& input) override;
    void getObjectsToRender(DisplayData &display) override;

    float getGameSpeed();
  };

}
