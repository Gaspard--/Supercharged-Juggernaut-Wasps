#pragma once

#include "Wasp.hpp"
#include "Bullet.hpp"
#include "Mob.hpp"

#include <vector>

struct AnimInfo
{
  claws::vect<float, 2u> destMin;
  claws::vect<float, 2u> destMax;
  uint32_t frame;
};

struct DisplayData
{
  std::optional<BigWasp> bigWasp{};
  std::optional<SmolWasp> smolWasp{};
  std::string stringedTime;
  uint32_t gameScore;
  std::vector<BulletInfo> bullets{};
  std::array<std::vector<AnimInfo>, size_t(SpriteId::SpriteCount)> anims;
};
