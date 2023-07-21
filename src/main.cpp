#include "Config.hpp"
#include "HTTPServer.hpp"
#include "Parser.hpp"
#include <fstream>
#include <iostream>
#include <istream>

void
debug () {
  std::cout << "TRIGGER | " << std::endl;
}

void
debugkey (std::string s) {
  std::cout << s << std::endl;
}

int
main () {
  std::fstream f ("config.conf");
  Parser p (f);
  AST a = p.parse();
  a.print ();
  return 0;
}