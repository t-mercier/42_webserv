#pragma once

#include <map>
#include <ostream>
#include <sstream>
#include <string>

class HTTPResponse {
public:
  HTTPResponse(int sock);
  HTTPResponse& write(std::string const& s);
  HTTPResponse& writef(std::string s, ...);
  HTTPResponse& header(std::string const& k, std::string const& v);
  std::stringstream& body();
  void end();

private:
  std::map<std::string, std::string> _headers;
  std::stringstream _body;
  int _sock;
};

std::ostream&
operator<<(std::ostream& o, HTTPResponse const& res);