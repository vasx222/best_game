#pragma once
#include "Singleton.h"
#include <iostream>
#include <fstream>

class Logger : public Singleton<Logger>
{
public:
  #define FILENAME "log.txt"
  template<typename T>
  Logger & operator << (T const & val)
  {
    std::cout << val;
    std::ofstream ofs(FILENAME, std::ofstream::out | std::ofstream::app);
    if (ofs.is_open())
    {
      ofs << val;
    }
    ofs.close();
    return *this;
  }
  static void InitLogFile()
  {
    std::ofstream ofs(FILENAME);
    ofs.close();
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
