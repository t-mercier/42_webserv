#include "Config.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "HTTPServer.hpp"
#include "Parser.hpp"
#include <fstream>
#include <iostream>
#include <istream>

void
debug() {
  std::cout << "TRIGGER | " << std::endl;
}

void
debugkey(std::string s) {
  std::cout << s << std::endl;
}

int
main() {

  std::fstream f("config.conf");
  Parser p(f);
  // p.parse ();

  HTTPServer server;

  server
    .on("/",
        [](HTTPRequest const& req, HTTPResponse& res) {
          res.header("testheader", "hihi");
          res.header("Content-Type", "text/html");
          res.body() << "<html><h1>Hello World</h1></html>\n";
          res.end();
        })
    .on("/hey",
        [](HTTPRequest const& req, HTTPResponse& res) {
          res.header("testheader", "hey header");
          res.header("Content-Type", "text/html");
          res.body() << "<html><h1>this is the /hey path</h1></html>\n";
          res.end();
        })
    .run();

  return 0;
}