#pragma once

# include <set>
# include <map>
# include <vector>
# include "CollisionSolver.hpp"
# include "claws/container/vect.hpp"
# include "Bullet.hpp"

namespace physic
{
  const uint32_t gridUnitSize = 50;

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

  template<class A, class B>
  void checkCollisionsEntities(A &a, B &b)
  {
    for (auto itA = a.begin() ; itA != a.end() ; ++itA)
      for (auto itB = b.begin() ; itB != a.end() ; ++itB)
	if (haveCollision(*itA, *itB))
	  CollisionSolver::solve(*itA, *itB);
  }

  template<class A>
  void checkCollisionsBullets(A &a,
			      std::map<claws::vect<uint32_t, 2u>, std::vector<uint32_t>> const &bulletIndexes,
			      std::vector<Bullet> &bullets)
  {
    std::set<uint32_t> usedBullets;
    for (auto itA = a.begin() ; itA != a.end() ; ++itA) {
      claws::vect<uint32_t, 2u> begin = {(itA->position[0] - itA->size) / gridUnitSize, (itA->position[1] - itA->size) / gridUnitSize};
      if (itA->position[0] - itA->size < 0)
	begin[0] = 0;
      if (itA->position[1] - itA->size < 0)
	begin[1] = 0;
      claws::vect<uint32_t, 2u> end = {(itA->position[0] + itA->size) / gridUnitSize, (itA->position[1] + itA->size) / gridUnitSize};
      claws::vect<uint32_t, 2u> itPos;
      for (itPos[0] = begin[0] ; itPos[0] <= end[0] ; ++itPos[0])
	for (itPos[1] = begin[1] ; itPos[1] <= end[1] ; ++itPos[1])
	  try
	    {
	      for (auto itBul = bulletIndexes.at(itPos).begin() ; itBul != bulletIndexes.at(itPos).end() ; ++itBul) {
		if (usedBullets.find(*itBul) == usedBullets.end())
		  continue;
		usedBullets.insert(*itBul);
		if (haveCollision(*itA, bullets[*itBul]))
		  CollisionSolver::solve(*itA, bullets[*itBul]);
	      }
	    } catch (std::out_of_range) {}
    }
  }
}
