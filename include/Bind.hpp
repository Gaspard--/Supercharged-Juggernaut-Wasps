#ifndef BIND_HPP
# define BIND_HPP

# include <algorithm>

template<class T>
class Bind
{
private:
  T t;
public:
  Bind(T t)
  {
    t.bind();
  }

  Bind(Bind<T> const &b) = delete;

  ~Bind()
  {
    t.unbind();
  }
};

#endif
