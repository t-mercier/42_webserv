#pragma once
#include <iostream>
#include <vector>

/*
 * --------------------------------------------| ENUM
 */
enum TOKEN {
  NONE,
  KEY,
  SEMI = ';',
  LB = '{',
  RB = '}',
};

/*
 * --------------------------------------------| Token
 */
class Token {
public:
  Token() = default;
  Token(TOKEN type, std::string value) : type(type), value(value){};
  ~Token(){};

  TOKEN type;
  std::string value;
};

/*
 * --------------------------------------------| AST
 */
class AST {
public:
  AST() = default;
  AST(std::string& value) : value(value){};
  ~AST() = default;

  std::string value;
  std::vector<AST> branches;
};

/*
 * --------------------------------------------| Configuration Parser
 */
class ConfigParser {
public:
  ConfigParser(std::istream& s) : stream(s), bracket(0){};
  ~ConfigParser() = default;

  AST createAST();

private:
  std::istream& stream;
  int bracket;
  Token getToken();
  AST parseConfig(int);

  /*
   ! Error
   */
  class Error : public std::runtime_error {
  public:
    explicit Error(const std::string& what) : std::runtime_error(what) {}
    explicit Error(const char* what) : std::runtime_error(what) {}
  };
};

std::ostream&
operator<<(std::ostream& o, const AST& rhs);
void
printAST(std::ostream& o, const AST& a, int level);