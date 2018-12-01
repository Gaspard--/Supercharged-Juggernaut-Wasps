#pragma once
#include <vector>
#include <array>
#include "claws/container/vect.hpp"

class Pattern
{
public:
  virtual ~Pattern() = default;
  virtual claws::vect<float, 2u> update(float speed) = 0;
};

class Straight : public Pattern
{
  claws::vect<float, 2u>  prvPos;
  claws::vect<float, 2u>  direct;

public:
  Straight(float ogX, float ogY, float endX, float endY);
  ~Straight() = default;
  claws::vect<float, 2u> update(float speed) override;
};

class SinCos : public Pattern
{
  claws::vect<float, 2u>  prvPos;
  claws::vect<float, 2u>  direct;
  uint                    lastUpdate;

public:
  SinCos(float ogX, float ogY, float endX, float endY);
  ~SinCos() = default;
  claws::vect<float, 2u> update(float speed) override;
};

class Waypoints : public Pattern
{
  claws::vect<float, 2u>              prvPos;
  std::vector<std::array<uint, 2u>>   pointsList;
  std::vector<claws::vect<float, 2u>> directList;

public:
  Waypoints(float ogX, float ogY, float endX, float endY);
  ~Waypoints() = default;
  claws::vect<float, 2u> update(float speed) override;
};
