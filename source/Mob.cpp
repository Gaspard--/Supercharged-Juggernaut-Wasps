#include "Mob.hpp"
#include "GameState.hpp"

void Mob::update(state::GameState &gameState)
{
  animationFrame += gameState.getGameSpeed() * 0.2f;
  ai->update(*this, gameState);
  position += speed * gameState.getGameSpeed();
}
