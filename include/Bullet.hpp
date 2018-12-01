#pragma once

#include "Entity.hpp"
#include "Pattern.hpp"
#include "claws/container/vect.hpp"

#include <memory>

#ifndef SMALL_BULLET
#define SMALL_BULLET 3
#endif

#ifndef MEDIUM_BULLET
#define MEDIUM_BULLET 5
#endif

#ifndef BIG_BULLET
#define BIG_BULLET 10
#endif

class Bullet : Entity
{

  float       speed;
  std::unique_ptr<Pattern> pattern;

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
  template<class PatternImpl>
  Bullet(float size, claws::vect<float, 2> position, std::unique_ptr<Pattern> &&pattern)
    : Entity(size, position)
    , pattern(std::move(pattern))
  {
  }

  ~Bullet() = default;
  void update(); // TODO
	
};
