#pragma once

#include "AI.hpp"

template<class ShotProvider>
class RepetitiveShotAi : public AI
{
  float time{0.0f};
  float interval;
  ShotProvider shotProvider;
public:
  template<class... Params>
  RepetitiveShotAi(float interval, Params &&... params)
    : interval(interval)
    , shotProvider{std::forward<Params>(params)...}
  {
  }

  virtual void update(Mob &mob, state::GameState &gameState) override
  {
    if (time > interval)
      {
	shotProvider(gameState, mob);
	time -= interval;
      }
    time += gameState.getGameSpeed();
  }
};
