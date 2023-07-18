#include "Parser.hpp"
#include <_ctype.h>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <istream>

Token::Token() : id(TOKEN::NONE), value(""){};

Token::Token(TOKEN _id, std::string _v) : id(_id), value(_v){};

Token Parser::getToken() {
  std::string key;
  char c;
  int sp = 0;
  stream >> std::ws;
  while (stream.good() && !std::isspace(stream.peek())) {
    stream >> c;
    switch (c) {
    case SEMI:
    case LB:
    case RB:

      return (Token((TOKEN)c, std::string(1, c)));
      // Token(KEY, key);
      //   return (Token(KEY, key));
    default: {
      key.push_back(c);
    }
    }
  }
  if (key.length())
    return (Token(KEY, key));
  return Token();
}

AST Parser::parse() {
  Token tk;
  AST ast;
  std::vector<AST> branch;

  while ((tk = getToken()).id != NONE) {
    switch (tk.id) {
    case NONE:
      break;
    case LB: {
      while ((tk = getToken()).id != LB) {
        parse();
        if (tk.id == SEMI)
          break;
      }
    }
    case SEMI:
    case RB:
      parse();
    case KEY:
      ast.leaf = tk.value;
      branch.push_back(ast);
      debugkey(tk.value);
      break;
    }
  }
  ast.branch = branch;
  return ast;
}

Parser::Parser(std::istream &s) : stream(s){};

std::ostream &operator<<(std::ostream &o, const AST &rhs) {
  o << std::endl;
  o << "  |__ name= " << rhs.leaf << std::endl;
}
