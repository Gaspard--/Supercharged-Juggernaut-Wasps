#pragma once

#include "Entity.hpp"

#include <optional>

struct BigWasp
{
  claws::vect<Entity, 2u> entities;
  claws::vect<float, 2u> speed;
};

struct SmolWasp : public Entity
{
  claws::vect<float, 2u> speed;
};
