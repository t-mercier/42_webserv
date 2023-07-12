#pragma once
#include "Config.hpp"
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
  std::vector<AST> value;
  std::string key;
};

class Token {
public:
  Token();
  Token(int token, std::string);
  int id;
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
