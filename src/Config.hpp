#pragma once
#include <istream>
#include <vector>

enum token {
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

class Config {
public:
  static Config &from(std::istream &s);
};

class Token {
public:
  Token();
  Token(int token, std::string);
  enum token id;
  std::string value;
};

class Parser {
public:
  Parser();
  Parser(std::istream &);
  Token getToken();
  AST parse();
  char getChar();

private:
  std::istream &stream;
};
