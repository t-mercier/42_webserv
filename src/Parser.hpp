#pragma once
#include "Config.hpp"
#include <istream>
#include <iostream>
#include <vector>
#include <cstdio>

enum TOKEN {
  NONE,
  KEY,
  SEMI = ';',
  LB = '{',
  RB = '}',
  EF,
};

class AST {
public:
  std::vector<AST> leaves;
  std::string value;
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
  Parser(std::istream &);
  Token getToken();
  AST parse();

private:
  std::istream &stream;
};

std::ostream &operator<<(std::ostream &, const AST &);
void debugkey(std::string);
void debug();