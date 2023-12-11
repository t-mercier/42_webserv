#pragma once

#include <functional>
#include <map>
#include <ostream>
#include <sstream>
#include <string>

class Response;
using f_res_error = std::function<void(Response& res, int code)>;

class Response {
public:
  Response();
  Response& write(std::string const& s);
  Response& header(std::string const& k, std::string const& v);
  std::stringstream& body();
  Response& code(int c);
  Response& status(std::string s);
  void end();
  static std::map<int, std::string> STATUS;
  static std::map<std::string, std::string> MIME;
  int done();
  bool isHandled();
  std::string& final();
  void begin();
  int getCode();
  void setErrorF(f_res_error f);
  void error(int c);

private:
  int _code;
  std::string _status;
  std::map<std::string, std::string> _headers;
  std::stringstream _body;
  std::string _final;
  int _end;
  bool _handled;
  f_res_error errorf;
};

std::ostream&
operator<<(std::ostream& o, Response const& res);