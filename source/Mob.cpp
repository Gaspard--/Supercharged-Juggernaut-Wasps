#include "Mob.hpp"
#include "GameState.hpp"

void Mob::update(state::GameState &gameState)
{
  ai->update(*this, gameState);
  position += speed * gameState.getGameSpeed();
}
