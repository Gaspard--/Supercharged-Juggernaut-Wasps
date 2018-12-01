#include "Bullet.hpp"

void Bullet::Update()
{
  this->position = this->pattern->Update(this->speed);
}