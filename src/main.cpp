#include "Config.hpp"
#include "HTTPServer.hpp"
#include <iostream>


int main() {
  HTTPServer server;
  server.run();
  return 0;
}