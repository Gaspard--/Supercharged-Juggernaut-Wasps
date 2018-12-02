#pragma once

#include "Entity.hpp"
#include "AI.hpp"

#include <memory>

namespace state
{
  class GameState;
}

class Mob : public Entity
{
public:
  claws::vect<float, 2u> speed;
  bool dead{false};
private:
  std::unique_ptr<AI> ai;
public:
  Mob(float size, claws::vect<float, 2> position, claws::vect<float, 2u> speed, std::unique_ptr<AI> &&ai)
    : Entity{size, position}
    , speed(speed)
    , ai(std::move(ai))
  {
  }

  void update(state::GameState &gameState);
};
