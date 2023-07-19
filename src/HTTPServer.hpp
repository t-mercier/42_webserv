#pragma once
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"

typedef void (*f_handler)(HTTPRequest const& req, HTTPResponse& res);

class HTTPServer {
public:
  HTTPServer();
  HTTPServer& on(std::string const& path, f_handler f);

  void run();

private:
  void loop();
  void create();
  void handleS(int);
  int sock;
  std::map<std::string, f_handler> handlers;
};
