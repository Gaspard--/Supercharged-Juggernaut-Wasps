#include "GameState.hpp"

namespace state
{
  GameState::GameState()
    : bigWasp{{Entity{0.2f, {0.0f, 0.0f}}, Entity{0.2f, {0.0f, 0.2f}}}, {0.0f, 0.001f}}
    , smolWasp{}
  {
  }

  float GameState::getGameSpeed()
  {
    if (smolWasp)
      return 0.1f;
    else
      return 1.0f;
  }

  StateType GameState::update()
  {
    for (Entity &entity : bigWasp.entities)
      entity.position += bigWasp.speed * getGameSpeed();
    if (smolWasp)
      smolWasp->position += smolWasp->speed * getGameSpeed();
    return StateType::CONTINUE;
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

  void GameState::getObjectsToRender(DisplayData &displayData)
  {
    displayData.bigWasp = bigWasp;
    displayData.smolWasp = smolWasp;
  }
}
