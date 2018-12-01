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

Straight::~Straight() {}

claws::vect<float, 2u> Straight::Update(float speed)
{
  this->prvPos[0] += this->direct[0] * speed; 
  this->prvPos[1] += this->direct[1] * speed;
  return this->prvPos;
}


// SinCos::SinCos(float ogX, float ogY, float endX, float endY)
// {

// }

// SinCos::~SinCos() {}

// claws::vect<float, 2u> SinCos::Update(float x, float y)
// {

// }


// Waypoints::Waypoints(float ogX, float ogY, float endX, float endY)
// {

// }

// Waypoints::~Waypoints(float x, float y)
// {
//   while (!this->directList.empty())
//     this->directList.pop_back();
//   while (!this->pointList.empty())
//     this->pointList.pop_back();
// }

// claws::vect<float, 2u> Waypoints::Update()
// {

// }

