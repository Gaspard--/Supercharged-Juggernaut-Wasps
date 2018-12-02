#pragma once
#include <vector>
#include <array>
#include "claws/container/vect.hpp"

class Bullet;

class Pattern
{
public:
  virtual ~Pattern() = default;
  virtual claws::vect<float, 2u> update(Bullet &, float speed) = 0;
};

class NoPattern : public Pattern
{
public:
  NoPattern() = default;
  claws::vect<float, 2u> update(Bullet &, float speed) override;
};

class SinCos : public Pattern
{
  float rate;
  float strength;
  float time;

public:
  SinCos(float rate, float strength, float time = 0.0f);
  ~SinCos() = default;
  claws::vect<float, 2u> update(Bullet &, float speed) override;
};

class Waypoints : public Pattern
{
  claws::vect<float, 2u>              prvPos;
  std::vector<std::array<uint32_t, 2u>>   pointsList;
  std::vector<claws::vect<float, 2u>> directList;

public:
  Waypoints(float ogX, float ogY, float endX, float endY);
  ~Waypoints() = default;
  claws::vect<float, 2u> update(Bullet &, float speed) override;
};
