
#include "Request.hpp"
#include <cctype>
#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string.h>
#include <string>
#include <vector>

Request::Request(Stream& stream) : state(0), stream(stream) {}

std::string
tolower(std::string& x) {
  std::string r;
  for (auto c : x) r.push_back(std::tolower(c));
  return r;
}

int
Request::parse_header() {
  std::string line = stream.rline();
  size_t p;
  if ((p = line.find(':')) != line.npos) {
    std::string key(line, 0, p);
    std::string val(line, p + 2);
    headers[tolower(key)] = val;
    return 1;
  }
  return 0;
}

std::vector<std::string>
split(const std::string& s, char delimiter) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);
  while (std::getline(tokenStream, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

std::string
urlDecode(const std::string& str) {
  std::string decoded;
  for (auto i = str.begin(); i != str.end(); ++i) {
    if (*i == '%') {
      if (std::distance(i, str.end()) >= 3) {
        int value;
        std::istringstream iss(std::string{ i + 1, i + 3 });
        if (iss >> std::hex >> value) {
          decoded.push_back(static_cast<char>(value));
          i += 2;
        }
      }
    } else if (*i == '+') {
      decoded.push_back(' ');
    } else {
      decoded.push_back(*i);
    }
  }
  return decoded;
}

std::map<std::string, std::string>
parseQueryString(const std::string& query) {
  std::map<std::string, std::string> data;
  auto pairs = split(query, '&');
  for (const auto& pair : pairs) {
    auto kv = split(pair, '=');
    if (kv.size() == 2) {
      data[urlDecode(kv[0])] = urlDecode(kv[1]);
    } else {
      data[urlDecode(kv[0])] = "";
    }
  }
  return data;
}

void
Request::parse_mpv() {
  method = stream.rword();
  if (stream.eof()) throw std::runtime_error("parse method");
  path = stream.rword();
  if (stream.eof()) throw std::runtime_error("parse path");
  version = stream.rword();
  stream.rline();
  auto i = path.find('?');
  if (i != path.npos) {
    query = parseQueryString(path.substr(i + 1));
    path = path.substr(0, i);
  }
  TRACE() << path << std::endl;
  path = urlDecode(path);
  TRACE() << path << std::endl;
}

void
Request::parse_head() {
  if (state > 0) return;
  parse_mpv();
  for (; parse_header();)
    ;
  headers.insert({ "content-length", "0" });
  state = 1;
}

void
Request::parse_body() {
  if (state > 1) return;
  body = stream.rrest();
  state = 2;
}

void
Request::parse() {
  parse_head();
  parse_body();
}

std::ostream&
operator<<(std::ostream& os, const Request& req) {
  os << req.method << "|" << req.path << "|" << req.version << std::endl;
  std::map<std::string, std::string>::const_iterator it;
  for (it = req.headers.begin(); it != req.headers.end(); ++it)
    os << it->first << ": " << it->second << std::endl;
  return os << req.body << std::endl;
}

std::string
Request::queryString() const {
  std::string r;
  for (auto& a : query) { r += a.first + "=" + a.second + "&"; }
  if (r.size()) r.pop_back();
  return r;
}