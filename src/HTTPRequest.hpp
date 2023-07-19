#pragma once
#include <map>
#include <string>

class HTTPRequest {
public:
  HTTPRequest (std::string const &);
  std::string method;
  std::string path;
  std::string version;
  std::map<std::string, std::string> headers;
  std::string body;

private:
  void parse (char *);
  char *parse_mpv (char *);
  char *parse_header (char *);
  char *parse_body (char *);
};

std::ostream &operator<< (std::ostream &, const HTTPRequest &);
