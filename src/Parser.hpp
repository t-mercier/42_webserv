#pragma once
#include <cstdio>
#include <iostream>
#include <istream>
#include <ostream>
#include <vector>
#include "Config.hpp"

enum TOKEN {
  NONE,
  KEY,
  SEMI = ';',
  LB = '{',
  RB = '}',
};

class AST {
 public:
  AST();

  std::vector<AST> branch;
  std::string leaf;

  void print() const;
  void printHelper(int) const;
//   friend std::ostream& operator<<(std::ostream&, const AST&);
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
  Parser(std::istream&);

  Token getToken();
  AST parse();

 private:
  std::istream& stream;
  AST ast;
};

void debugkey(std::string);
void debug();
