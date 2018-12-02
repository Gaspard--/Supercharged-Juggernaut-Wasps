#pragma once

#include "Entity.hpp"

#include <optional>

static constexpr float maxInvuln  = 100.0f; // time in ms

struct BigWasp
{
  claws::vect<Entity, 2u> entities;
  claws::vect<float, 2u> speed;
  float invulnFrames{0.0f};
};

struct SmolWasp : public Entity
{
  claws::vect<float, 2u> speed;
  uint32_t dieCounter{0};
};
