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

struct RotatedAnimInfo : public AnimInfo
{
  claws::vect<float, 2u> dir;
};

struct ColorInfo
{
  claws::vect<float, 2u> destMin;
  claws::vect<float, 2u> destMax;
  claws::vect<float, 4u> color;
};

struct DisplayData
{
  std::optional<BigWasp> bigWasp{};
  std::optional<SmolWasp> smolWasp{};
  std::vector<std::pair<std::string, std::string>> deadFellows;
  bool gameOverHud{false};
  float timer;
  std::string stringedTime;
  uint32_t gameScore;
  std::vector<ColorInfo> colors{};
  std::array<std::vector<AnimInfo>, size_t(SpriteId::SpriteCount)> anims;
  std::array<std::vector<RotatedAnimInfo>, size_t(SpriteId::SpriteCount)> rotatedAnims;
  float screenShake;
};
