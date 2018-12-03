#include "Wasp.hpp"

#include <iostream>

void BigWasp::update(float gameSpeed) noexcept
{
  constexpr std::array<float, 3u> const flow{{0.05f, 0.02f, 0.002f}};
  constexpr std::array<float, 3u> const ratio{{0.25f, 0.25f, 0.5f}};
  std::array<float, 3u> area{{entities[0].size * entities[0].size,
			      entities[1].size * entities[1].size,
			      entities[2].size * entities[2].size}};
  float lastDiff((area[2] - size * size * ratio[2]) * flow[2] * gameSpeed);
  area[2] -= lastDiff;
  size = std::sqrt(size * size + lastDiff * 0.8f);
  for (size_t i(0ul); i != entities.size() - 1; ++i)
    {
      float diff((area[i] - size * size * ratio[i]) * flow[i] * gameSpeed);

      area[i + 1] += diff;
      area[i] += lastDiff * 0.4f - diff;
    }
  for (size_t i(0ul); i != entities.size(); ++i)
    {
      entities[i].size = std::sqrt(area[i]);
    }
  speed *= std::pow(0.7f, gameSpeed);
  entities[0].position += speed * gameSpeed;
  for (auto it(entities.begin() + 1); it != entities.end(); ++it)
    {
      it->position[0] += speed[0] * gameSpeed;
      it->position[1] = (it - 1)->position[1] - ((it - 1)->size + it->size);
    }
}