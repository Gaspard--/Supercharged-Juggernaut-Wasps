#pragma once
#include <vector>
#include <array>
#include "claws/container/vect.hpp"

class Pattern
{
public:
  virtual claws::vect<float, 2u> Update(float speed);
};

class Straight : Pattern
{
  claws::vect<float, 2u>  prvPos;
  claws::vect<float, 2u>  direct;

public:
  Straight(float ogX, float ogY, float endX, float endY)
    : prvPos{ogX, ogY},
      direct{endX - ogX, endY - ogY}
  {}
  claws::vect<float, 2u> Update(float speed);
};

class SinCos : Pattern
{
  claws::vect<float, 2u>  prvPos;
  claws::vect<float, 2u>  direct;
  uint                    lastUpdate;

public:
  SinCos(float ogX, float ogY, float endX, float endY)
    : prvPos{ogX, ogY},
      direct{endX - ogX, endY - ogY},
      lastUpdate{0}
  {}
  claws::vect<float, 2u> Update(float speed);
};

class Waypoints : Pattern
{
  claws::vect<float, 2u>              prvPos;
  std::vector<std::array<uint, 2u>>   pointsList;
  std::vector<claws::vect<float, 2u>> directList;

public:
  Waypoints(float ogX, float ogY, float endX, float endY);
  ~Waypoints();
  claws::vect<float, 2u> Update(float speed);
};