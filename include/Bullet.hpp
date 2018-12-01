#pragma once

#include "Entity.hpp"
#include "Pattern.hpp"
#include "claws/container/vect.hpp"

#include <memory>


static constexpr uint small_bullet  = 3u;
static constexpr uint medium_bullet = 5u;
static constexpr uint big_bullet    = 10u;

class Bullet : Entity
{
  float                     speed;
  std::unique_ptr<Pattern>  pattern;

public:
  template <typename P>
  Bullet( float ogX, float ogY,
          float endX, float endY,
          uint size = medium_bullet)
    : Entity(size, {ogX, ogY}),
      pattern{new P(ogX, ogY, endX, endY)}
    {}
  ~Bullet();
  void Update();
};