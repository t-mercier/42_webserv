#pragma once
#include <istream>
#include <vector>

class Config {
public:
  static Config &from (std::istream &s);
};
