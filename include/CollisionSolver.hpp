#pragma once

# include <typeindex>

class CollisionSolver
{

public:

  // template<class T, class U>
  // static void solve(T &t, U &u)
  // {
  //   if constexpr (typeid(T) > typeid(U))
  //     solver(t, u);
  //   else
  //     solver(u, t);
  // }
  void solve(Bullet const &bullet, Entity const &entity)
  {
  }

private:
  //default
  // template<class A, class B>
  // static void solver(A &, B &)
  // {
  //   //do_nothing
  // }

  //
  // include here different effect you want to have redefining 'solver'
  //

};
