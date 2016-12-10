#pragma once
#include "Singleton.h"
#include <iostream>
#include <fstream>
#include <list>

class Logger : public Singleton<Logger>
{
public:
  #define LOGFILENAME "log.txt"
  template<typename T>
  Logger & operator << (T const & val)
  {
    std::cout << val;
//    std::ofstream ofs(LOGFILENAME, std::ofstream::out | std::ofstream::app);
//    if (ofs.is_open())
//    {
//      ofs << val;
//    }
//    ofs.close();
    return *this;
  }
  static void InitLogFile()
  {
    std::ofstream ofs(LOGFILENAME);
    ofs.close();
  }
private:
  friend class Singleton<Logger>;
  Logger() = default;
};
