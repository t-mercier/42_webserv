#include "Parser.hpp"
#include <algorithm>
#include <iostream>
#include <istream>

Token::Token() : id(TOKEN::NONE), value(""){};

Token::Token(TOKEN _id, std::string _v) : id(_id), value(_v){};

Token Parser::getToken() {
  std::string key;
  char c;
  while (stream >> c && std::isspace(c))
    ;
  do {
    if (!(stream >> c))
      return Token(TOKEN::EF, "");
    TOKEN token;
switch (c) {
    case ';':
        token = SEMI;
        break;
    case '{':
        token = LB;
        break;
    case '}':
        token = RB;
        break;
    default:
        token = KEY;
}
  } while (!std::isspace(c));
  debug();
  return Token();
}

AST Parser::parse() {
  Token tk;
  AST ast;
  std::vector<AST> leaves;

  while ((tk = getToken()).id != EF) {
    debugkey(tk.value);

    if (tk.id == KEY) {
      ast.value = tk.value;
      leaves.push_back(ast);
      continue;
    }
  }
  ast.leaves = leaves;
  std::cout << ast;
  return ast;
}

Parser::Parser(std::istream &s) : stream(s){};

std::ostream &operator<<(std::ostream &stream, const AST &ast) {
  stream << std::endl;
  // while (ast.leaves.size())
  stream << "name=" << ast.value;
  stream << " | " << std::endl;
}
