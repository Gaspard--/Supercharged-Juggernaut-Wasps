#include "Pattern.hpp"
#include "Bullet.hpp"
#include "claws/container/vect.hpp"

claws::vect<float, 2u> Straight::Update(float speed)
{
  this->prvPos[0] += this->direct[0] * speed; 
  this->prvPos[1] += this->direct[1] * speed;
  return this->prvPos;
}

claws::vect<float, 2u> SinCos::Update(float speed)
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



Waypoints::~Waypoints()
{
  while (!this->directList.empty())
    this->directList.pop_back();
  while (!this->pointsList.empty())
    this->pointsList.pop_back();
}

claws::vect<float, 2u> Waypoints::Update(float speed)
{
  this->prvPos[0] += this->directList[0][0] * speed; 
  this->prvPos[1] += this->directList[0][1] * speed;


  return this->prvPos;
}

