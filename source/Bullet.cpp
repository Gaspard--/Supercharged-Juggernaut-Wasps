#include "Bullet.hpp"

#include <iostream>

void Bullet::update(float gameSpeed)
{
  std::visit([&](auto &renderData) noexcept
	     {
	       if constexpr (std::is_same_v<std::decay_t<decltype(renderData)>, BulletSprite>)
	         renderData.frame += gameSpeed * 0.2f;
	     }, renderData);
  this->position += this->pattern->update(*this, gameSpeed) + this->speed * gameSpeed;
}
