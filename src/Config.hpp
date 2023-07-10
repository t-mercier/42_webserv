#pragma once
#include <istream>

class Config {

public:
  static Config &from(std::istream& s);
};