#pragma once
#include <vector>
#include <array>
#include "claws/container/vect.hpp"

class Straight
{
  claws::vect<float, 2u>  prvPos;
  claws::vect<float, 2u>  direct;

public:
  const bool is_pattern = true;
  Straight(float ogX, float ogY, float endX, float endY);
  ~Straight();
  claws::vect<float, 2u> Update(float speed);
};

class SinCos
{
  claws::vect<float, 2u>  prvPos;
  claws::vect<float, 2u>  direct;

public:
  const bool is_pattern = true;
  SinCos(float ogX, float ogY, float endX, float endY);
  ~SinCos();
  claws::vect<float, 2u> Update(float speed);
};

class Waypoints
{
  claws::vect<float, 2u>              prvPos;
  std::vector<std::array<uint, 2u>>   pointsList;
  std::vector<claws::vect<float, 2u>> directList;

public:
  const bool is_pattern = true;
  Waypoints(float ogX, float ogY, float endX, float endY);
  ~Waypoints();
  claws::vect<float, 2u> Update(float speed);
};