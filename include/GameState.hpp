#pragma once

#include <optional>
#include <vector>

#include "State.hpp"
#include "Wasp.hpp"
#include "Bullet.hpp"
#include "Mob.hpp"

namespace state
{
  class GameState : public State
  {
    std::vector<Bullet> bullets;
    std::vector<Mob> mobs;
    BigWasp bigWasp;
    std::optional<SmolWasp> smolWasp;
    claws::vect<float, 2u> target;
    bool gotoTarget{false};
    float gameSpeed;
    float spawnTimer{0.0f};

    void makeCollisions();
    void spawnWave();

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
