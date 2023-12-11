#include "Stream.hpp"


Stream::Stream() : rp(0) {}

void
Stream::write(std::string const& a) {
  append(a);
}

char
Stream::rchar() {
  if (rp == length()) return EOF;
  return at(rp++);
}

std::string
Stream::rword() {
  for (; !eof() && std::isspace(peek(1).front());) rchar();
  std::string a;
  for (; !eof() && !std::isspace(peek(1).front());) a.push_back(rchar());
  return a;
}

std::string
Stream::rline() {
  std::string a;
  for (; !eof() && peek(1) != "\n";) a.push_back(rchar());
  rchar();
  return a;
}

std::string
Stream::rrest() {
  std::string a = substr(rp);
  rp += a.length();
  return a;
}

std::string
Stream::peek(int n) {
  return substr(rp, n);
}

bool
Stream::eof() {
  return length() == rp;
}

class NullBuffer : public std::streambuf {
public:
  int overflow(int c) {
    return c; // indicate success
  }
};

NullBuffer null_buffer;
std::ostream null_stream(&null_buffer);

std::ostream&
TRACE() {
#ifdef DEBUG
  return std::cerr;
#else
  return null_stream;
#endif
}