#include "GameState.hpp"
#include "Physic.hpp"
#include "RepetitiveShotAI.hpp"
#include "SoundHandler.hpp"

#include <iostream>
#include <algorithm>

namespace state
{
  GameState::GameState()
    : bigWasp{{Entity{0.03f, {0.0f, 0.0f}}, Entity{0.04f, {0.0f, -0.06f}}}, {0.0f, 0.0f}}
    , smolWasp{}
  {
    for (auto i = jsButtonWasPressed.begin() ; i != jsButtonWasPressed.end() ; ++i)
      *i = false;
  }

  float GameState::getGameSpeed()
  {
    return gameSpeed;
  }

  void GameState::makeCollisions()
  {
    std::map<claws::vect<uint32_t, 2u>, std::vector<uint32_t>> bulletIndexes;
    for (uint32_t i = 0 ; i != bullets.size() ; ++i) {
      claws::vect<uint32_t, 2u> begin = {uint32_t((bullets[i].position[0] - bullets[i].size) / physic::gridUnitSize),
					 uint32_t((bullets[i].position[1] - bullets[i].size) / physic::gridUnitSize)};
      claws::vect<uint32_t, 2u> end = {uint32_t((bullets[i].position[0] + bullets[i].size) / physic::gridUnitSize),
				       uint32_t((bullets[i].position[1] + bullets[i].size) / physic::gridUnitSize)};
      claws::vect<uint32_t, 2u> itPos;
      for (itPos[0] = begin[0] ; itPos[0] != end[0] + 1; ++itPos[0])
	for (itPos[1] = begin[1] ; itPos[1] != end[1] + 1; ++itPos[1])
	  bulletIndexes[itPos].push_back(i);
    }
    for (auto &entity : bigWasp.entities)
      physic::checkCollisionsBullets(bulletIndexes, entity, bullets, [&](auto &, Bullet &bullet)
								     {
								       if (bigWasp.invulnFrames <= 0.0f)
									 bigWasp.invulnFrames = maxInvuln;
								       bullet.dead = true;
								       SoundHandler::getInstance().playSound(SoundHandler::waspTakeHit);
								       std::cout << "hit!" << std::endl;
								     });
    if (bigWasp.invulnFrames <= 0.0f)
      {
	physic::checkCollisionsEntities(bigWasp.entities[0], mobs, [](auto &, Mob &mob)
								   {
								     SoundHandler::getInstance().playSound(SoundHandler::mobTakeHit);
								     mob.dead = true;
								     std::cout << "munch\n";
								   });
      }
    if (smolWasp)
      {
	if (smolWasp->dieCounter)
	  {
	    ++smolWasp->dieCounter;
	    smolWasp->size *= 1.1f;
	  }
	if (smolWasp->dieCounter == 15)
	  {
	    static constexpr float noScaleLimit(0.016f);
	    smolWasp->size = 0.4f;
	    physic::checkCollisionsBullets(bulletIndexes, *smolWasp, bullets, [](auto &smolWasp, Bullet &bullet)
									      {
										auto diff(bullet.position - smolWasp.position);
										if (diff.length2() < noScaleLimit)
										  bullet.speed += diff * 0.0001f / (std::sqrt(diff.length2()) * noScaleLimit);
										else
										  bullet.speed += diff * 0.0001f / diff.length2();
									      });
	    physic::checkCollisionsEntities(*smolWasp, mobs, [](auto &smolWasp, Mob &mob)
							     {
							       auto diff(mob.position - smolWasp.position);
							       if (diff.length2() < noScaleLimit)
								 mob.speed += diff * 0.0001f / (std::sqrt(diff.length2()) * noScaleLimit);
							       else
								 mob.speed += diff * 0.0001f / (diff.length2());
							     });
	    smolWasp.reset();
	  }
      }
  }

  void GameState::spawnWave()
  {
    class VShots
    {
    public:
      VShots() = default;

      void operator()(GameState &gameState, Mob &mob)
      {
	for (float i(0.1f); i < 2.5f; i += 1.0f)
	  {
	    gameState.bullets.emplace_back(0.007f,
					   mob.position,
					   claws::vect<float, 2u>{i * 0.0004f, -0.0006f * (5.0F - i)},
					   claws::vect{0.0f, 1.0f, 1.0f, 1.0f},
					   std::make_unique<NoPattern>());
	    gameState.bullets.emplace_back(0.007f,
					   mob.position,
					   claws::vect<float, 2u>{-i * 0.0004f, -0.0006f * (5.0F - i)},
					   claws::vect{0.0f, 1.0f, 1.0f, 1.0f},
					   std::make_unique<NoPattern>());
	  }
      }
    };
    if (rand() % 24 == 0)
      {
	mobs.emplace_back(0.02f,
			  claws::vect<float, 2u>{-1.0f, 0.5f},
			  claws::vect<float, 2u>{0.003f, 0.0007f},
			  std::make_unique<RepetitiveShotAi<VShots>>(120.0f));
      }
    if (rand() % 24 == 0)
      {
	mobs.emplace_back(0.02f,
			  claws::vect<float, 2u>{1.0f, 0.5f},
			  claws::vect<float, 2u>{-0.003f, 0.0007f},
			  std::make_unique<RepetitiveShotAi<VShots>>(120.0f));
      }
    class RotateShots
    {
      float angle{0.0f};
    public:
      RotateShots() = default;

      void operator()(GameState &gameState, Mob &mob)
      {
	angle += 0.3f;
	gameState.bullets.emplace_back(0.005f,
				       mob.position,
				       claws::vect<float, 2u>{sin(angle), cos(angle)} * 0.002f,
				       claws::vect{1.0f, 0.0f, 0.5f, 1.0f},
				       std::make_unique<NoPattern>());
      }
    };
    if (rand() % 24 == 0)
      {
	mobs.emplace_back(0.05f,
			  claws::vect<float, 2u>{0.0f, 0.99f},
			  claws::vect<float, 2u>{0.0f, -0.0007f},
			  std::make_unique<RepetitiveShotAi<RotateShots>>(30.0f));
      }
  }

  StateType GameState::update()
  {
    gameSpeed *= 0.98f;
    gameSpeed += 0.02f * (smolWasp ? 0.3f : 1.0f);
    SoundHandler::getInstance().setGlobalPitch(getGameSpeed());

    constexpr float const spawnInterval{30.0f};

    spawnTimer += getGameSpeed();
    if (spawnTimer > spawnInterval)
      {
	spawnTimer -= spawnInterval;
	spawnWave();
      }

    if (bigWasp.invulnFrames > 0)
      {
	bigWasp.invulnFrames -= getGameSpeed();
      }

    if (smolWasp)
      {
	smolWasp->animationFrame += getGameSpeed() * 0.5;
	if (gotoTarget)
	  smolWasp->speed += (target - smolWasp->position) * 0.5f * getGameSpeed() * getGameSpeed();
	else if (joystickInUse)
	  smolWasp->speed = joystickVect * 0.08f;
	smolWasp->speed *= std::pow(0.7f, getGameSpeed());
      }
    else
      {
	if (gotoTarget)
	  bigWasp.speed += (target - bigWasp.entities[0].position) * 0.01f * getGameSpeed() * getGameSpeed();
	else if (joystickInUse)
	  bigWasp.speed = joystickVect * 0.008f;
      }
    bigWasp.speed *= std::pow(0.7f, getGameSpeed());
    for (Entity &entity : bigWasp.entities)
      entity.position += bigWasp.speed * getGameSpeed();
    if (smolWasp)
      smolWasp->position += smolWasp->speed * getGameSpeed();
    for (auto &mob : mobs)
      mob.update(*this);
    for (auto &bullet : bullets)
      bullet.update(getGameSpeed());
    makeCollisions();
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet const &bullet)
								 {
								   if (bullet.dead)
								     return true;
								   for (uint32_t i(0u); i != 2; ++i)
								     if (bullet.position[i] > 1.0f || bullet.position[i] < -1.0f)
								       return true;
								   return false;
								 }), bullets.end());
    mobs.erase(std::remove_if(mobs.begin(), mobs.end(), [](Mob const &mob)
							{
							  if (mob.dead)
							    return true;
							  for (uint32_t i(0u); i != 2; ++i)
							    if (mob.position[i] > 1.0f || mob.position[i] < -1.0f)
							      return true;
							  return false;
							}), mobs.end());
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
	  makeSmolExplode();
	else
	  spawnSmol();
      }
  }

  void GameState::checkEvents(input::Input &input)
  {
    gotoTarget = (input.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT));
    std::vector<claws::vect<float, 2>> axes = input.getJoystickAxes();
    std::vector<unsigned char> buttons = input.getJoystickButtons();
    if (axes.size() && axes[0].length2() > 0.1) {
      joystickInUse = true;
      gotoTarget = false;
      joystickVect = {axes[0][0], axes[0][1] * -1};
    } else {
      joystickInUse = false;
    }
    if (buttons.size()) {
      if (buttons[0] == GLFW_PRESS && !jsButtonWasPressed[0]) {
	jsButtonWasPressed[0] = true;
	if (smolWasp)
	  makeSmolExplode();
	else
	  spawnSmol();
      }
      for (unsigned i = 0 ; i < buttons.size() && i < jsButtonWasPressed.size() ; ++i)
	if (buttons[i] == GLFW_RELEASE)
	  jsButtonWasPressed[i] = false;
    }
  }

  void GameState::makeSmolExplode()
  {
    smolWasp->dieCounter = true;
  }

  void GameState::spawnSmol()
  {
    smolWasp.emplace(SmolWasp{Entity{0.01f, bigWasp.entities[1].position}, {0.0f, 0.0f}});
  }

  void GameState::getObjectsToRender(DisplayData &displayData)
  {
    displayData.bigWasp = bigWasp;
    displayData.smolWasp = smolWasp;
    for (auto const &bullet : bullets)
      displayData.bullets.emplace_back(static_cast<BulletInfo>(bullet));
    for (auto const &mob : mobs)
      displayData.mobs.emplace_back(static_cast<Entity>(mob));
  }
}
