#pragma once

#include "Entity.hpp"
#include "Pattern.hpp"
#include "SpriteManager.hpp"
#include "claws/container/vect.hpp"

#include <memory>
#include <variant>

static constexpr uint8_t small_bullet  = 3u;
static constexpr uint8_t medium_bullet = 5u;
static constexpr uint8_t big_bullet    = 10u;

struct BulletSprite
{
  SpriteId spriteId;
  float frame{0.0f};
};

struct BulletInfo : public Entity
{
  claws::vect<float, 2u> speed;
  std::variant<claws::vect<float, 4u>, BulletSprite> renderData;
};

class Bullet : public BulletInfo
{
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
  bool dead{false};

  Bullet(float size, claws::vect<float, 2> position, claws::vect<float, 2> speed, claws::vect<float, 4> color, std::unique_ptr<Pattern> &&pattern)
    : BulletInfo{Entity{size, position}, speed, {color}}
    , pattern(std::move(pattern))
  {
  }

  Bullet(float size, claws::vect<float, 2> position, claws::vect<float, 2> speed, SpriteId spriteId, std::unique_ptr<Pattern> &&pattern)
    : BulletInfo{Entity{size, position}, speed, {BulletSprite{spriteId}}}
    , pattern(std::move(pattern))
  {
  }

  Bullet(Bullet &&) = default;
  Bullet& operator=(Bullet &&) = default;

  ~Bullet() = default;
  void update(float gameSpeed);
};
