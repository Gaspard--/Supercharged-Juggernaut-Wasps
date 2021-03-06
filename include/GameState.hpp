#pragma once

#include <optional>
#include <vector>
#include <array>
#include <map>

#include "State.hpp"
#include "Wasp.hpp"
#include "Bullet.hpp"
#include "Mob.hpp"
#include "DeadFellows.hpp"
#include "Gore.hpp"

namespace state
{
  class GameState : public State
  {
    std::vector<Bullet> bullets;
    std::vector<Mob> mobs;
    std::vector<Gore> gores;
    BigWasp bigWasp;
    std::optional<SmolWasp> smolWasp;
    claws::vect<float, 2u> target;
    bool gotoTarget{false};
    bool joystickInUse{false};
    claws::vect<float, 2u> joystickVect;
    std::array<bool, 1> jsButtonWasPressed;
    std::vector<Mob> boss;
    uint32_t gameScore{0};
    std::vector<std::pair<std::string, std::string>> deadFellows;

    float gameSpeed{1.0f};
    float timer{0.0f};
    float lastSpawn{0.0f};
    bool bossSpawned{false};
    float lastScore{0.0f};
    float screenShake{0.0f};
    float winCounter{0.0f};
    bool won{false};

    void makeCollisions();
    void spawnWave();

    void makeSmolExplode();
    void spawnSmol();
    void explosion(std::map<claws::vect<uint32_t, 2u>, std::vector<uint32_t>> const &bulletIndexes, claws::vect<float, 2u> position, float power);


  public:
    GameState();
    StateType update(unsigned int &time) override;
    void handleKey(GLFWwindow *window, input::Key key) override;
    void handleMouse(input::Input const &, GLFWwindow *window, input::Mouse mouse) override;
    void handleButton(GLFWwindow *window, input::Button button) override;
    void checkEvents(input::Input& input) override;
    void getObjectsToRender(DisplayData &display) override;

    float getGameSpeed();
  };

}
