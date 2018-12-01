#pragma once

#include <optional>
#include <vector>

#include "State.hpp"
#include "Wasp.hpp"
#include "Bullet.hpp"

namespace state
{
  class GameState : public State
  {
    std::vector<Bullet> bullets;
    BigWasp bigWasp;
    std::optional<SmolWasp> smolWasp;
    claws::vect<float, 2u> target;
    bool gotoTarget{false};

    void makeCollisions();

  public:
    GameState();
    StateType update() override;
    void handleKey(GLFWwindow *window, input::Key key) override;
    void handleMouse(input::Input const &, GLFWwindow *window, input::Mouse mouse) override;
    void handleButton(GLFWwindow *window, input::Button button) override;
    void checkEvents(input::Input& input) override;
    void getObjectsToRender(DisplayData &display) override;

    float getGameSpeed();
  };

}
