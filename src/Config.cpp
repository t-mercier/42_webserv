#include "Config.hpp"
#include <cctype>
#include <string>

Config &Config::from(std::istream &s) {
  Config *c = new Config();

  return *c;
}

Token::Token() : id(NONE), value(""){};
Token::Token(int _id, std::string _v) : id(_id), value(_v){};

Token Parser::getToken() {
  std::string key;
  char c;
  while (stream >> c && std::isspace(c))
    ;
  do {
    switch (c) {
    case SEMI:
    case LB:
    case RB:
      return (Token(c, std::string(1, c)));
    default:
      key.push_back(c);
    }
  } while (stream >> c && !std::isspace(c));
  if (key.length())
    return (Token(KEY, key));
  return Token();
}

AST Parser::parse() {
  Token tk;
  AST ast;
  while ((tk = getToken()).id) {
    {
      if (tk.id == KEY) {
        ast.key = tk.value;
        continue;
      }
      if (tk.id == RB) {
        while (parse())
          if (tk.id != LB)
            break ;
        }
    }
  }
}
