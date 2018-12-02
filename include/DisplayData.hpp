#pragma once

#include "Wasp.hpp"
#include "Bullet.hpp"

#include <vector>

struct DisplayData
{
  std::optional<BigWasp> bigWasp{};
  std::optional<SmolWasp> smolWasp{};
  std::vector<BulletInfo> bullets{};
  std::vector<Entity> mobs{};
};
