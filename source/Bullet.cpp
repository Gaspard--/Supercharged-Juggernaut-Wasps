#include "Bullet.hpp"

#include <iostream>

void Bullet::update(float gameSpeed)
{
  this->position += this->pattern->update(*this, gameSpeed) + this->speed * gameSpeed;
}
