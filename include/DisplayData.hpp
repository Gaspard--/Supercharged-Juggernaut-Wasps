#pragma once

#include "Wasp.hpp"

#include <vector>

struct DisplayData
{
  std::optional<BigWasp> bigWasp{};
  std::optional<SmolWasp> smolWasp{};
  std::vector<Entity> bullets{};
};
