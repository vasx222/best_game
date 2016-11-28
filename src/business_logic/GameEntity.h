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
};
