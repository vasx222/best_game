#pragma once
#include <typeinfo>
#include <iostream>
#include "patterns/logger.h"

class GameEntity
{
public:
  virtual ~GameEntity()
  {
    Logger::Instance() << "Destructor " << *this << "\n";
  }

  friend std::ostream & operator << (std::ostream & os, GameEntity const & obj)
  {
    os << typeid(obj).name();
    return os;
  }
  template<typename T, template<typename, typename...> class C, typename... Args>
  friend std::ostream & operator << (std::ostream & os, C<T, Args...> const & objs)
  {
    os << "Collection: ";
    for (auto const & obj : objs)
      os << obj << ' ';
    os << "\n";
    return os;
  }
};
