#include "Bullet.hpp"

void Bullet::update()
{
  this->position = this->pattern->update(this->speed);
}
