#pragma once
#include "Config.hpp"
#include <cstdio>
#include <iostream>
#include <istream>
#include <vector>

enum TOKEN {
  NONE,
  KEY,
  SEMI = ';',
  LB = '{',
  RB = '}',
};

class AST {
public:
  std::vector<AST> branch;
  std::string leaf;

  friend std::ostream& operator<<(std::ostream&, const AST&);
};

class Token {
public:
  Token();
  Token(TOKEN token, std::string);
  TOKEN id;
  std::string value;
};

class Parser {
public:
  Parser();
  Parser(std::istream&);
  Token getToken();
  AST parse();

private:
  std::istream& stream;
};

void debugkey(std::string);
void
debug();