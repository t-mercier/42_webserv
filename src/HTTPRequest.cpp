
#include "HTTPRequest.hpp"
#include <cctype>
#include <cstring>
#include <iostream>
#include <string.h>

HTTPRequest::HTTPRequest (std::string const &s) { parse ((char *)s.c_str ()); }

char *
HTTPRequest::parse_body (char *s) {
  body = std::string (s);
  return s + strlen (s);
}

char *
HTTPRequest::parse_header (char *s) {
  char *x = std::strchr (s, ':');
  if (!x)
    return s;
  std::string key (s, x - s);
  std::string val (x + 1, std::strchr (x, '\r') - x);
  headers[key] = val;
  return std::strchr (s, '\n') + 1;
}

char *
HTTPRequest::parse_mpv (char *s) {
  char *x = strchr (s, ' ');
  method = std::string (s, x - s);
  s = x;
  x = strchr (x, ' ');
  path = std::string (s, x - s);
  s = x;
  x = strchr (x, '\r');
  version = std::string (s, x - s);
  return std::strchr (s, '\n') + 1;
}

void
HTTPRequest::parse (char *s) {
  s = parse_mpv (s);
  for (char *x = s; x < (s = parse_header (s)); x = s)
    ;
  s = parse_body (s);
}

std::ostream &
operator<< (std::ostream &os, const HTTPRequest &req) {
  os << req.method << " " << req.path << " " << req.version << std::endl;
  std::map<std::string, std::string>::const_iterator it;
  for (it = req.headers.begin (); it != req.headers.end (); ++it)
    os << it->first << ": " << it->second << std::endl;
  return os << req.body << std::endl;
}