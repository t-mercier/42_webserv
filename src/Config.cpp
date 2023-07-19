#include "Config.hpp"
#include <cctype>
#include <string>

Config&
Config::from(std::istream& s) {
  Config* c = new Config();

  return *c;
}
