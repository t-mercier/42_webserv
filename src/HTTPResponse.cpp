#include "HTTPResponse.hpp"
#include <cstdarg>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <unistd.h>

HTTPResponse::HTTPResponse(int sock)
  : _sock(sock) {}

HTTPResponse&
HTTPResponse::write(std::string const& s) {
  _body << (s);
  return *this;
}

HTTPResponse&
HTTPResponse::header(std::string const& k, std::string const& v) {
  _headers[k] = v;
  return *this;
}

int
writestr(int fd, std::string const& r) {
  return write(fd, r.c_str(), r.length());
}

void
HTTPResponse::end() {
  header("Connection", "close");
  header("Content-Length", std::to_string(_body.str().size()));
  std::string status = "HTTP/1.1 200 OK\r\n";
  writestr(_sock, status);
  std::map<std::string, std::string>::iterator it;
  for (it = _headers.begin(); it != _headers.end(); ++it) {
    std::string header = it->first + ": " + it->second + "\r\n";
    writestr(_sock, header);
  }
  ::write(_sock, "\r\n", 2);
  writestr(_sock, _body.str());
  close(_sock);
}

HTTPResponse&
HTTPResponse::writef(std::string fmt, ...) {

  va_list v, w;
  va_start(v, fmt);
  va_copy(w, v);
  int n = vsnprintf(0, 0, fmt.c_str(), v);
  std::string buf;
  buf.resize(n);
  vsnprintf(&buf[0], n + 1, fmt.c_str(), w);
  va_end(w);
  va_end(v);

  _body << buf;

  return *this;
}

std::stringstream&
HTTPResponse ::body() {
  return _body;
}

std::ostream&
operator<<(std::ostream& o, HTTPResponse const& res) {
  return o;
}
