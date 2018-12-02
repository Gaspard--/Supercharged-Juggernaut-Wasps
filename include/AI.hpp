#pragma once

namespace state
{
  class GameState;
}
class Mob;

class AI
{
public:
  virtual ~AI() = default;
  virtual void update(Mob &mob, state::GameState &) = 0;
};
