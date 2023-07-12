#include "HTTPServer.hpp"
#include "sys/socket.h"

HTTPServer::HTTPServer(){};

void HTTPServer::accept() {
  int c = socket(AF_INET, SOCK_STREAM, 0);
  if (c < 0)
    throw "aaaaaa";
}