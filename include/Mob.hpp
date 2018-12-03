#pragma once

#include "Entity.hpp"
#include "AI.hpp"
#include "SpriteManager.hpp"

#include <memory>

namespace state
{
  class GameState;
}

enum class Behavior
  {
   LookAtPlayer,
   LookForward,
   LookDown,
   NoRotation
  };

struct MobInfo : public Entity
{
  claws::vect<float, 2u> speed;
  SpriteId spriteId;
  Behavior behavior;
  float animationFrame{0.0f};
};

class Mob : public MobInfo
{
public:
  bool dead{false};
private:
  std::unique_ptr<AI> ai;
public:
  Mob(float size, claws::vect<float, 2> position, claws::vect<float, 2u> speed, SpriteId spriteId, Behavior behavior, std::unique_ptr<AI> &&ai)
    : MobInfo{{size, position}, speed, spriteId, behavior}
    , ai(std::move(ai))
  {
  }

  void update(state::GameState &gameState);
};
