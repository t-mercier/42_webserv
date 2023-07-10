#include "Config.hpp"

Config &Config::from(std::istream &s) {
  Config *c = new Config();
  return *c;
}