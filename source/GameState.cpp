#include "GameState.hpp"
#include "Physic.hpp"
#include "RepetitiveShotAI.hpp"
#include "SoundHandler.hpp"

#include <iostream>
#include <algorithm>

namespace state
{
  static constexpr claws::vect<float, 2u> downRotation{0.0f, -1.0f};

  GameState::GameState()
    : bigWasp{{Entity{0.0f, {0.0f, 0.0f}}, Entity{0.0f, {0.0f, 0.00f}}, Entity{0.15f, {0.0f, -0.01f}}}, {0.0f, 0.0f}}
    , smolWasp{}
  {
    for (uint32_t i(0); i < 10000; ++i)
      bigWasp.update(1);
    for (auto i = jsButtonWasPressed.begin() ; i != jsButtonWasPressed.end() ; ++i)
      *i = false;
  }

  float GameState::getGameSpeed()
  {
    return gameSpeed;
  }

  void GameState::explosion(std::map<claws::vect<uint32_t, 2u>, std::vector<uint32_t>> const &bulletIndexes, claws::vect<float, 2u> position, float power)
  {
    static constexpr float noScaleLimit(0.04f);

    Entity entity{power, position};

    physic::checkCollisionsBullets(bulletIndexes, entity, bullets, [](auto &entity, Bullet &bullet)
								      {
									auto diff(bullet.position - entity.position);
									if (diff.length2() < noScaleLimit * noScaleLimit)
									  diff /= (std::sqrt(diff.length2()) * noScaleLimit);
									else
									  diff /= diff.length2();
									bullet.speed += diff * 0.001f * entity.size; 
									bullet.position += diff * 0.002f * entity.size;
								      });
    physic::checkCollisionsEntities(entity, mobs, [](auto &entity, Mob &mob)
						     {
						       auto diff(mob.position - entity.position);

						       if (diff.length2() < noScaleLimit * noScaleLimit)
							 diff /= (std::sqrt(diff.length2()) * noScaleLimit);
						       else
							 diff /= diff.length2();
						       mob.speed += diff * 0.001f * entity.size; 
						       mob.position += diff * 0.002f * entity.size;
						       mob.size *= 0.9f;
						     });
    physic::checkCollisionsEntities(entity, boss, [](auto &entity, Mob &mob)
						  {
						    mob.size *= 0.9f;
						  });
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
									 {
									   SoundHandler::getInstance().playSound(SoundHandler::waspTakeHit);
									   bigWasp.invulnFrames = maxInvuln;
									   bigWasp.size *= BigWasp::hitPenality;
									   for (auto &entity : bigWasp.entities)
									     entity.size *= BigWasp::hitPenality;
									 }
								       bullet.dead = true;
								     });
    physic::checkCollisionsEntities(bigWasp.entities[0], mobs, [this](auto &, Mob &mob)
							       {
								 if (mob.size < bigWasp.size)
								   {
								     SoundHandler::getInstance().playSound(SoundHandler::mobTakeHit);
								     mob.dead = true;
								     gameScore += uint32_t(mob.size / bigWasp.size * 100);
								     float delta(std::sqrt(physic::square(bigWasp.entities[2].size) + physic::square(mob.size) * 2.0f) - bigWasp.entities[2].size);
								     bigWasp.entities[0].size += delta;
								   }
							       });
    physic::checkCollisionsEntities(bigWasp.entities[0], boss, [this](auto &, Mob &mob)
							       {
								 if (mob.size < bigWasp.size)
								   {
								     SoundHandler::getInstance().playSound(SoundHandler::mobTakeHit);
								     mob.dead = true;
								     gameScore += uint32_t(mob.size / bigWasp.size * 100);
								     float delta(std::sqrt(physic::square(bigWasp.entities[2].size) + physic::square(mob.size) * 2.0f) - bigWasp.entities[2].size);
								     bigWasp.entities[0].size += delta;
								   }
							       });
    if (smolWasp)
      {
	if (smolWasp->dieCounter)
	  {
	    ++smolWasp->dieCounter;
	  }
	if (smolWasp->dieCounter == 15)
	  {
	    explosion(bulletIndexes, smolWasp->position, smolWasp->size * 20.0f);
	    smolWasp.reset();
	  }
      }
  }

  void GameState::spawnWave()
  {
    gameScore += uint32_t(bigWasp.size * 1000.f);
    class VShots
    {
      float spreadMax;
    public:
      VShots(float spreadMax)
	: spreadMax(spreadMax - 1.5f)
      {}

      void operator()(GameState &gameState, Mob &mob)
      {
	for (float i(0.1f); i < spreadMax; i += 1.0f)
	  {
	    gameState.bullets.emplace_back(0.01f,
					   mob.position,
					   claws::vect<float, 2u>{i * 0.0004f, -0.0006f * (5.0F - i)},
					   SpriteId::Fireball,
					   std::make_unique<NoPattern>());
	    gameState.bullets.emplace_back(0.01f,
					   mob.position,
					   claws::vect<float, 2u>{-i * 0.0004f, -0.0006f * (5.0F - i)},
					   SpriteId::Fireball,
					   std::make_unique<NoPattern>());
	  }
      }
    };
    float size(std::sqrt(float(rand() % 16 + 1)));
    float power(boss.empty() ? size : size * 0.5f); // mobs are less strong when boss is there

    if (rand() % 24 == 0)
      {
	mobs.emplace_back(0.01f * size,
			  claws::vect<float, 2u>{-1.0f, 0.99f},
			  claws::vect<float, 2u>{0.003f, -0.0003f - float(rand() % 4) * 0.0001f},
			  SpriteId::Libeflux,
			  Behavior::LookForward,
			  std::make_unique<RepetitiveShotAi<VShots>>(120.0f, power));
      }
    if (rand() % 24 == 0)
      {
    	mobs.emplace_back(0.01f * size,
			  claws::vect<float, 2u>{1.0f, 0.99f},
			  claws::vect<float, 2u>{-0.003f, -0.0003f - float(rand() % 4) * 0.0001f},
			  SpriteId::Libeflux,
			  Behavior::LookForward,
			  std::make_unique<RepetitiveShotAi<VShots>>(120.0f, power));
      }
    class RotateShots
    {
      float angle{0.0f};
    public:
      RotateShots() = default;

      void operator()(GameState &gameState, Mob &mob)
      {
	angle += 0.3f;
	gameState.bullets.emplace_back(0.01f,
				       mob.position,
				       claws::vect<float, 2u>{sin(angle), cos(angle)} * 0.002f,
				       claws::vect{1.0f, 0.0f, 0.5f, 1.0f},
				       std::make_unique<NoPattern>());
      }
    };
    if (rand() % 24 == 0)
      {
	mobs.emplace_back(0.08f,
			  claws::vect<float, 2u>{-0.9f + float(rand() % 19) * 0.1f, 0.99f},
			  claws::vect<float, 2u>{0.0f, -0.0007f},
			  SpriteId::Monarch,
			  Behavior::LookDown,
			  std::make_unique<RepetitiveShotAi<RotateShots>>(boss.empty() ? 30.0f : 120.0f)); // lower fire rate when boss present
      }
    class BossAi : public AI
    {
      float time{0.0f};
      float interval;
      uint32_t phaseCounter{0u};
      uint32_t phase{0u};

    public:
      BossAi(float interval)
	: interval(interval)
      {
      }

      virtual void update(Mob &mob, state::GameState &gameState) override
      {
	auto speed(gameState.bigWasp.entities[1].position - mob.position);
	claws::vect<float, 2u> side({speed[1], -speed[0]});
	if (time > interval)
	  {
	    if (!(++phaseCounter %= 10))
	      ++phase %= 2;
	    switch(phase)
	      {
	      case 0:
		for (float i(0.1f); i < 2.5; i += 1.0f)
		  {
		    gameState.bullets.emplace_back(0.04f,
						   mob.position,
						   side * i * 0.0004f + speed * 0.0006f * (5.0f - i),
						   SpriteId::Fireball,
						   std::make_unique<NoPattern>());
		    gameState.bullets.emplace_back(0.04f,
						   mob.position,
						   -side * i * 0.0004f + speed * 0.0006f * (5.0f - i),
						   SpriteId::Fireball,
						   std::make_unique<NoPattern>());
		  }
		break;
	      case 1:
		gameState.bullets.emplace_back(0.02f,
					       mob.position,
					       speed * 0.012f,
					       SpriteId::Fireball,
					       std::make_unique<NoPattern>());
		break;

	      }
	    time -= interval;
	  }
	time += gameState.getGameSpeed();
	mob.speed *= std::pow(0.9f, gameState.getGameSpeed());
	mob.speed += speed * 0.0001f * gameState.getGameSpeed();
      }
    };


    if (!bossSpawned && bigWasp.size > 0.15f)
      {
	bossSpawned = true;
	for (auto &mob : mobs)
	  {
	    mob.speed += mob.position * 0.01f / mob.position.length2();
	  }
	boss.emplace_back(0.2f,
			  claws::vect<float, 2u>{0.0f, 1.2f},
			  claws::vect<float, 2u>{0.0f, -0.0007f},
			  SpriteId::SmolWasp,
			  Behavior::LookAtPlayer,
			  std::make_unique<BossAi>(50.0f));
      }
      
  }

  StateType GameState::update(unsigned int &time)
  {
    gameSpeed *= 0.98f;
    gameSpeed += 0.02f * (smolWasp ? 0.3f : 1.0f);
    SoundHandler::getInstance().setGlobalPitch(getGameSpeed());

    constexpr float const spawnInterval{30.0f};

    timer += getGameSpeed();
    if (timer > lastSpawn + spawnInterval)
      {
	lastSpawn += spawnInterval;
	spawnWave();
      }

    if (bigWasp.invulnFrames > 0)
      {
	bigWasp.invulnFrames -= getGameSpeed();
      }

    if (smolWasp)
      {
	smolWasp->animationFrame += getGameSpeed() /* 0.5f*/;
	if (gotoTarget)
	  smolWasp->speed += (target - smolWasp->position) * 0.1f * getGameSpeed() * getGameSpeed();
	else if (joystickInUse)
	  smolWasp->speed = joystickVect * 0.03f;
	smolWasp->speed *= std::pow(0.7f, getGameSpeed());
      }
    else
      {
	if (gotoTarget)
	  bigWasp.speed += (target - bigWasp.entities[0].position) * 0.01f * getGameSpeed() * getGameSpeed();
	else if (joystickInUse)
	  bigWasp.speed = joystickVect * 0.008f;
      }
    bigWasp.update(getGameSpeed());
    if (smolWasp)
      smolWasp->position += smolWasp->speed * getGameSpeed();
    for (auto &mob : mobs)
      mob.update(*this);
    for (auto &mob : boss)
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
    boss.erase(std::remove_if(boss.begin(), boss.end(), [](Mob const &mob)
							{
							  return mob.dead;
							}), boss.end());
    if (bigWasp.size < BigWasp::minSize)
      return GAME_OVER_STATE;
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
      joystickVect = {axes[0][0], axes[0][1]
#ifndef _WIN32
		      * -1
#endif
      };
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
    smolWasp.emplace(SmolWasp{Entity{bigWasp.size * std::sqrt(0.1f), bigWasp.entities[1].position}, {0.0f, 0.0f}});
    bigWasp.size *= std::sqrt(0.9f);
    for (auto &entity : bigWasp.entities)
      entity.size *= std::sqrt(0.9f);
  }

  void GameState::getObjectsToRender(DisplayData &displayData)
  {
    displayData.gameScore = gameScore;
    displayData.bigWasp = bigWasp;
    displayData.smolWasp = smolWasp;
    displayData.timer = timer;
    for (auto const &bullet : bullets)
      std::visit([&](auto const &renderData)
		 {
		   if constexpr (std::is_same_v<std::decay_t<decltype(renderData)>, BulletSprite>)
		     displayData.rotatedAnims[size_t(renderData.spriteId)].emplace_back(RotatedAnimInfo{{bullet.position - bullet.size * 2.0f,
													 bullet.position + bullet.size * 2.0f,
													 uint32_t(renderData.frame)},
													bullet.speed});
		   else
		     displayData.colors.emplace_back(ColorInfo{bullet.position - bullet.size,
							       bullet.position + bullet.size,
							       renderData});
		 }, bullet.renderData);
    auto mobConvert([&](auto const &mob)
		    {
		      switch (mob.behavior)
			{
			case Behavior::NoRotation:
			  displayData.anims[size_t(mob.spriteId)].emplace_back(AnimInfo{mob.position - mob.size * 2.0f,
											mob.position + mob.size * 2.0f,
											uint32_t(mob.animationFrame)});
			  break;
			case Behavior::LookForward:
			  displayData.rotatedAnims[size_t(mob.spriteId)].emplace_back(RotatedAnimInfo{{mob.position - mob.size * 2.0f,
												       mob.position + mob.size * 2.0f,
												       uint32_t(mob.animationFrame)}, mob.speed});
			  break;
			case Behavior::LookAtPlayer:
			  displayData.rotatedAnims[size_t(mob.spriteId)].emplace_back(RotatedAnimInfo{{mob.position - mob.size * 2.0f,
												       mob.position + mob.size * 2.0f,
												       uint32_t(mob.animationFrame)},
												      mob.position - bigWasp.entities[1].position});
			  break;
			case Behavior::LookDown: // 180.0f, 0.0f
			  displayData.rotatedAnims[size_t(mob.spriteId)].emplace_back(RotatedAnimInfo{{mob.position - mob.size * 2.0f,
												       mob.position + mob.size * 2.0f,
												       uint32_t(mob.animationFrame)},
												      downRotation});
			  break;
			default:
			  ;
			}
		      if (mob.size < bigWasp.size)
			{
			  claws::vect<float, 2u> offset(0.0f, (std::sin(mob.animationFrame * 0.4f) + 1.0f) * mob.size * 0.2f);
			  displayData.rotatedAnims[size_t(SpriteId::Target)].emplace_back(RotatedAnimInfo{{mob.position - mob.size * 2.0f + offset,
													   mob.position + mob.size * 2.0f + offset,
													   uint32_t(mob.animationFrame)},
													  claws::vect<float, 2>{std::sin(mob.animationFrame * 0.1f), cos(mob.animationFrame * 0.1f)}
			    });
			}
		    });
    for (auto const &mob : mobs)
      mobConvert(mob);
    for (auto const &mob : boss)
      mobConvert(mob);
  }
}
