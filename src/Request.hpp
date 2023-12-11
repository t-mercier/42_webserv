#pragma once
#include "Stream.hpp"
#include <map>
#include <string>

class Server;
class Request {
public:
  Request(Stream& stream);
  std::string method;
  std::string path;
  std::string version;
  std::map<std::string, std::string> headers;
  std::string body;
  std::map<std::string, std::string> query;
  Server* server;
  std::string queryString() const;

  void parse_head();
  void parse_body();

private:
  void parse();
  void parse_mpv();
  int parse_header();
  int state;
  Stream& stream;
};

std::ostream&
operator<<(std::ostream&, const Request&);
