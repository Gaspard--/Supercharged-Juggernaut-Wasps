#include "Pattern.hpp"
#include "Bullet.hpp"
#include "claws/container/vect.hpp"

claws::vect<float, 2u> NoPattern::update(Bullet &, float speed)
{
  return {0.0f, 0.0f};
}


SinCos::SinCos(float rate, float strength, float time)
  : rate(rate)
  , strength(strength)
  , time(time)
{
}

claws::vect<float, 2u> SinCos::update(Bullet &bullet, float speed)
{
  time += speed;
  return claws::vect<float, 2u>{bullet.speed[1], -bullet.speed[0]} * cos(time * 3.14f * rate) * strength * speed;
}

Waypoints::Waypoints(float ogX, float ogY, float endX, float endY)
{

}

claws::vect<float, 2u> Waypoints::update(Bullet &, float speed)
{
  this->prvPos[0] += this->directList[0][0] * speed; 
  this->prvPos[1] += this->directList[0][1] * speed;


  return this->prvPos;
}

