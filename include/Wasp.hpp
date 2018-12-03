#pragma once

#include "Entity.hpp"

#include <optional>

static constexpr float maxInvuln  = 100.0f; // time in ms

struct BigWasp
{
  static constexpr float minSize = 0.02f;
  static constexpr float hitPenality = std::sqrt(0.5f);

  claws::vect<Entity, 3u> entities;
  claws::vect<float, 2u> speed;
  float invulnFrames{0.0f};
  float size{0.00f};

  void update(float speed) noexcept;
};

struct SmolWasp : public Entity
{
  claws::vect<float, 2u> speed;
  float animationFrame{0.0f};
  uint32_t dieCounter{0};
};
