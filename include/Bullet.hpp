#pragma once

#include "Entity.hpp"
#include "Pattern.hpp"
#include "claws/container/vect.hpp"

#include <memory>

#include <iostream>

static constexpr uint small_bullet  = 3u;
static constexpr uint medium_bullet = 5u;
static constexpr uint big_bullet    = 10u;

class Bullet : public Entity
{ 
public:
  claws::vect<float, 2u> speed;
private:
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
  Bullet(float size, claws::vect<float, 2> position, claws::vect<float, 2> speed, std::unique_ptr<Pattern> &&pattern)
    : Entity{size, position}
    , speed(speed)
    , pattern(std::move(pattern))
  {
  }

  Bullet(Bullet &&) = default;
  Bullet& operator=(Bullet &&) = default;

  ~Bullet() = default;
  void update(float gameSpeed);

};
