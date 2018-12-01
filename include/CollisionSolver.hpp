#pragma once

# include <typeindex>

class CollisionSolver
{

public:

  template<class T, class U>
  static void solve(T &t, U &u)
  {
    if (std::type_index(typeid(T)) > std::type_index(typeid(U)))
      solver(t, u);
    else
      solver(u, t);
  }

private:
  //default
  template<class A, class B>
  static void solver(A &, B &)
  {
    //do_nothing
  }

  //
  // include here different effect you want to have redefining 'solver'
  //

};
