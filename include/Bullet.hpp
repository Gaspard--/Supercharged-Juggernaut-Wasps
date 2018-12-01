#pragma once
#include "Entity.hpp"
#include "claws/container/vect.hpp"

#ifndef SMALL_BULLET
#define SMALL_BULLET 3
#endif

#ifndef MEDIUM_BULLET
#define MEDIUM_BULLET 5
#endif

#ifndef BIG_BULLET
#define BIG_BULLET 10
#endif

template <class Pattern>
class Bullet : Entity
{
  static_assert(Pattern::is_pattern,
    "Bullet didn't received a Pattern");

  float       speed;
  Pattern     *pattern;

  // ideas :
  // follow a straight line:
  //   set a point and draw a vector from origin to this point
  // follow a sin/cos:
  //   set a equation f(x) with x pixel parcoured from origin (in height only)
  //   change vector every 5/10 pixel
  // movement then follow straight line:
  //   set Bullet on a path then if a certain height is crossed
  //   stop changing vector
  // follow waypoints:
  //   set some points (max 5) then draw rect from one to another and follow


public:
  Bullet(float x, float y) {
    this->size        = MEDIUM_BULLET;
    this->position[0] = x;
    this->position[1] = y;
  }
  ~Bullet();
  void Update();
	
};