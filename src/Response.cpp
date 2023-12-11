#include "Response.hpp"

#include <cstdarg>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>

std::map<int, std::string> Response::STATUS = {
  { 100, "Continue" },
  { 101, "Switching Protocols" },
  { 200, "OK" },
  { 201, "Created" },
  { 202, "Accepted" },
  { 203, "Non-Authoritative Information" },
  { 204, "No Content" },
  { 205, "Reset Content" },
  { 206, "Partial Content" },
  { 300, "Multiple Choices" },
  { 301, "Moved Permanently" },
  { 302, "Found" },
  { 303, "See Other" },
  { 304, "Not Modified" },
  { 305, "Use Proxy" },
  { 307, "Temporary Redirect" },
  { 400, "Bad Request" },
  { 401, "Unauthorized" },
  { 402, "Payment Required" },
  { 403, "Forbidden" },
  { 404, "Not Found" },
  { 405, "Method Not Allowed" },
  { 406, "Not Acceptable" },
  { 407, "Proxy Authentication Required" },
  { 408, "Request Timeout" },
  { 409, "Conflict" },
  { 410, "Gone" },
  { 411, "Length Required" },
  { 412, "Precondition Failed" },
  { 413, "Request Entity Too Large" },
  { 414, "Request-URI Too Long" },
  { 415, "Unsupported Media Type" },
  { 416, "Requested Range Not Satisfiable" },
  { 417, "Expectation Failed" },
  { 500, "Internal Server Error" },
  { 501, "Not Implemented" },
  { 502, "Bad Gateway" },
  { 503, "Service Unavailable" },
  { 504, "Gateway Timeout" },
  { 505, "Version Not Supported" }
};

std::map<std::string, std::string> Response::MIME = {
  { "html", "text/html" },
};

Response::Response() : _code(200), _end(0), _handled(false) {}

bool
Response::isHandled() {
  return _handled;
}

void
Response::begin() {
  _handled = true;
}

Response&
Response::write(std::string const& s) {
  _body << (s);
  return *this;
}

Response&
Response::header(std::string const& k, std::string const& v) {
  _headers[k] = v;
  return *this;
}

void
Response::end() {
  begin();
  header("Connection", "close");
  header("Content-Length", std::to_string(_body.str().size()));
  std::string status = "HTTP/1.1 " + std::to_string(_code) + " " +
                       (this->_status.empty() ? STATUS[_code] : this->_status) +
                       "\r\n";
  _final.append(status);
  for (auto& it : _headers) _final.append(it.first + ": " + it.second + "\r\n");
  _final.append("\r\n");
  _final.append(_body.str());
  _end = 1;
}

std::stringstream&
Response::body() {
  return _body;
}

std::ostream&
operator<<(std::ostream& o, Response const& res) {
  (void)res;
  return o;
}

Response&
Response::code(int c) {
  this->_code = c;
  return *this;
}

Response&
Response::status(std::string s) {
  this->_status = s;
  return *this;
}

int
Response::done() {
  return _end;
}

std::string&
Response::final() {
  return _final;
}

int
Response::getCode() {
  return _code;
}

void
Response::setErrorF(f_res_error f) {
  errorf = f;
}

void
Response::error(int c) {
  errorf(*this, c);
}