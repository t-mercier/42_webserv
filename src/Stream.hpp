#pragma once
#include <cctype>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <thread>

class Stream : public std::string {
 public:
  Stream();

  void write(std::string const& a);
  std::string rword();
  std::string rline();
  std::string rrest();
  char rchar();
  std::string peek(int n);
  bool eof();

 private:
  size_t rp;
};

std::ostream& TRACE();