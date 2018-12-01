#include "Pattern.hpp"
#include "Bullet.hpp"
#include "claws/container/vect.hpp"

Straight::Straight( float ogX, float ogY,
                    float endX, float endY)
{
  this->prvPos[0] = ogX;
  this->prvPos[1] = ogY;
  this->direct[0] = endX - ogX;
  this->direct[1] = endY - ogY;
}

claws::vect<float, 2u> Straight::update(float speed)
{
  this->prvPos[0] += this->direct[0] * speed;
  this->prvPos[1] += this->direct[1] * speed;
  return this->prvPos;
}

SinCos::SinCos(float ogX, float ogY, float endX, float endY)
{

}

claws::vect<float, 2u> SinCos::update(float speed)
{
  if (this->lastUpdate %5 == 0)
  {
    claws::vect<float, 2u> tmpDirect{std::sin(static_cast<float>(this->lastUpdate)), 1.0f};
    this->direct += tmpDirect;
  }

  this->prvPos[0] += this->direct[0] * speed; 
  this->prvPos[1] += this->direct[1] * speed;
  this->lastUpdate++;

  this->lastUpdate %= 90;

  return this->prvPos;
}

Waypoints::Waypoints(float ogX, float ogY, float endX, float endY)
{

}

claws::vect<float, 2u> Waypoints::update(float speed)
{
  this->prvPos[0] += this->directList[0][0] * speed; 
  this->prvPos[1] += this->directList[0][1] * speed;


  return this->prvPos;
}

