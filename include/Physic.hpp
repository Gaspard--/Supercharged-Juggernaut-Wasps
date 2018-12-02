#pragma once

# include <set>
# include <map>
# include <vector>
# include "claws/container/vect.hpp"
# include "Bullet.hpp"

namespace physic
{
  constexpr float const gridUnitSize = 0.2f;

  template<class T>
  constexpr T square(T const &t)
  {
    return t * t;
  }

  template<class A, class B>
  bool haveCollision(const A& a, const B& b)
  {
    return (a.position - b.position).length2() < square(a.size + b.size);
  }

  template<class B>
  bool haveCollision(claws::vect<float, 2u> const& a, B const& b)
  {
    return (a - b.position).length2() < square(b.size);
  }

  template<class EntityType, class B, class Solver>
  void checkCollisionsEntities(EntityType &entity, B &bs, Solver &&solver)
  {
    for (auto &b : bs)
      if (haveCollision(entity, b))
	solver(entity, b);
  }

  template<class EntityType, class Solver>
  void checkCollisionsBullets(std::map<claws::vect<uint32_t, 2u>, std::vector<uint32_t>> const &bulletIndexes,
			      EntityType &entity,
			      std::vector<Bullet> &bullets,
			      Solver &&solver)
  {
    std::set<uint32_t> usedBullets;
    claws::vect<uint32_t, 2u> begin({uint32_t((entity.position[0] - entity.size) / gridUnitSize),
				     uint32_t((entity.position[1] - entity.size) / gridUnitSize)});
    claws::vect<uint32_t, 2u> end({uint32_t((entity.position[0] + entity.size) / gridUnitSize),
				   uint32_t((entity.position[1] + entity.size) / gridUnitSize)});
    claws::vect<uint32_t, 2u> itPos;
    for (itPos[0] = begin[0] ; itPos[0] != end[0] + 1; ++itPos[0])
      for (itPos[1] = begin[1] ; itPos[1] != end[1] + 1; ++itPos[1])
	try
	  {
	    for (auto bulletIndex : bulletIndexes.at(itPos)) {
	      if (usedBullets.find(bulletIndex) != usedBullets.end())
		continue;
	      usedBullets.insert(bulletIndex);
	      if (haveCollision(entity, bullets[bulletIndex]))
		solver(entity, bullets[bulletIndex]);
	    }
	  } catch (std::out_of_range const &) {}
  }
}
