#include "Config.hpp"
#include "HTTPServer.hpp"
#include <iostream>
#include <istream>
#include <fstream>
#include "Parser.hpp"

void debug() { std::cout << "TRIGGER | " << std::endl; }

void debugkey(std::string s) {
  std::cout << s << " | ";
  }

int main() {
  std::fstream f("config.conf");
  Parser p(f);
  p.parse();
  return 0;
}