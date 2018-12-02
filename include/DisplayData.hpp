#pragma once

#include "Wasp.hpp"
#include "Bullet.hpp"
#include "Mob.hpp"

#include <vector>

struct DisplayData
{
  std::optional<BigWasp> bigWasp{};
  std::optional<SmolWasp> smolWasp{};
  std::vector<BulletInfo> bullets{};
  std::vector<MobInfo> mobs{};
};
