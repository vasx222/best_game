#pragma once
#include "Singleton.h"
#include <iostream>

class Logger : public Singleton<Logger>
{
public:
  template<typename T>
  Logger & operator << (T const & val)
  {
    std::cout << val;
    return *this;
  }
private:
  friend class Singleton<Logger>;
  Logger() = default;
};

template<typename T, template<typename, typename...> class C, typename... Args>
std::ostream & operator << (std::ostream & os, C<T, Args...> const & objs)
{
  os << "Collection: ";
  for (auto const & obj : objs)
    os << obj << ' ';
  os << "\n";
  return os;
}
