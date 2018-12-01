#include "GameState.hpp"
#include "Physic.hpp"

namespace state
{
  GameState::GameState()
    : bigWasp{{Entity{0.1f, {0.0f, 0.0f}}, Entity{0.1f, {0.0f, -0.2f}}}, {0.0f, 0.0f}}
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

  void GameState::makeCollisions()
  {
    std::map<claws::vect<uint32_t, 2u>, std::vector<uint32_t>> bulletIndexes;
    for (uint32_t i = 0 ; i != bullets.size() ; ++i) {
      claws::vect<uint32_t, 2u> begin = {(bullets[i].position[0] - bullets[i].size) / physic::gridUnitSize,
					 (bullets[i].position[1] - bullets[i].size) / physic::gridUnitSize};
      if (bullets[i].position[0] - bullets[i].size < 0)
	begin[0] = 0;
      if (bullets[i].position[1] - bullets[i].size < 0)
	begin[1] = 0;
      claws::vect<uint32_t, 2u> end = {(bullets[i].position[0] + bullets[i].size) / physic::gridUnitSize,
				       (bullets[i].position[1] + bullets[i].size) / physic::gridUnitSize};
      claws::vect<uint32_t, 2u> itPos;
      for (itPos[0] = begin[0] ; itPos[0] <= end[0] ; ++itPos[0])
	for (itPos[1] = begin[1] ; itPos[1] <= end[1] ; ++itPos[1])
	  bulletIndexes[itPos].push_back(i);
    }
    physic::checkCollisionsBullets(bigWasp.entities, bulletIndexes, bullets);
  }

  StateType GameState::update()
  {
    if (smolWasp)
      {
	if (gotoTarget)
	  smolWasp->speed += (target - smolWasp->position) * 0.5f * getGameSpeed() * getGameSpeed();
	smolWasp->speed *= std::pow(0.7f, getGameSpeed());
      }
    else
      {
	if (gotoTarget)
	  bigWasp.speed += (target - bigWasp.entities[0].position) * 0.01f * getGameSpeed() * getGameSpeed();
      }
    bigWasp.speed *= std::pow(0.9f, getGameSpeed());
    for (Entity &entity : bigWasp.entities)
      entity.position += bigWasp.speed * getGameSpeed();
    if (smolWasp)
      smolWasp->position += smolWasp->speed * getGameSpeed();
    makeCollisions();
    return StateType::CONTINUE;
  }

  void GameState::handleKey(GLFWwindow *, input::Key)
  {
  }

  void GameState::handleMouse(input::Input const &input, GLFWwindow *, input::Mouse mouse)
  {
    target = claws::vect<float, 2>{float(mouse.x), float(mouse.y)} * 2.0f;
    target -= claws::vect_cast<float>(input.getSize());
    target /= float(input.getSize()[1]);
    target[1] *= -1.0f;
  }

  void GameState::handleButton(GLFWwindow *, input::Button button)
  {
    if (button.button == GLFW_MOUSE_BUTTON_RIGHT && button.action == GLFW_PRESS)
      {
	if (smolWasp)
	  smolWasp.reset();
	else
	  smolWasp.emplace(SmolWasp{Entity{0.01f, bigWasp.entities[1].position}, {0.0f, 0.0f}});
      }
  }

  void GameState::checkEvents(input::Input &input)
  {
    gotoTarget = (input.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT));
  }

  void GameState::getObjectsToRender(DisplayData &displayData)
  {
    displayData.bigWasp = bigWasp;
    displayData.smolWasp = smolWasp;
  }
}
