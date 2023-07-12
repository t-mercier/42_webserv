#pragma once
#include <istream>
#include <vector>

enum tk {
  NONE,
  KEY,
  SEMI = ';',
  LB = '{',
  RB = '}',
};


class Config {
public:
  static Config &from(std::istream &s);
};

