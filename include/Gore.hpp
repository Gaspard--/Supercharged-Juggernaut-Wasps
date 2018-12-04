#include "Entity.hpp"

struct Gore : public Entity
{
  float maxFrames{3.0f};
  float animationFrame{0.0f};
  Gore(float size, claws::vect<float, 2> position)
  : Entity{size, position}
  {}
};