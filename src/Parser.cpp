#include "Parser.hpp"
#include <_ctype.h>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <istream>

/*_____________________________ constructors _____________________________*/

Token::Token()
  : id(TOKEN::NONE)
  , value(""){};

Token::Token(TOKEN _id, std::string _v)
  : id(_id)
  , value(_v){};

/*______________________________ destructor ______________________________*/

/*_______________________________ overloads ______________________________*/

std::ostream&
operator<<(std::ostream& o, AST const& rhs) {
  o << rhs.value << ' ';
  return o;
}

/*_______________________________ accessors ______________________________*/

std::string
AST::getLeaf() {
  return value;
}

/*________________________________ methods _______________________________*/

Token
Parser::getToken() {
  std::string key;
  char c;
  char b;
  stream >> std::ws;
  while (stream.good() && !std::isspace(stream.peek())) {
    switch (stream.peek()) {
      case SEMI:
        return (stream >> c, Token((TOKEN)c, std::string(1, c)));
      case LB:
      case RB:
        return (stream >> c, Token((TOKEN)c, std::string(1, c)));
      default:
        stream >> c;
        key.push_back(c);
        if (stream.peek() == ';' && key.length())
          return (Token(KEY, key));
    }
  }
  if (key.length())
    return (Token(KEY, key));
  return Token();
}

AST
Parser::parse() {
  Token u;
  AST a, ast;
    std::vector<AST> v;
  //   std::vector<AST> branch;

  while ((u = getToken()).id != NONE) {
    switch (u.id) {
      case NONE:
      case RB:
        break;
      case LB:
      case SEMI: // ignore semicolons, key-value pair already saved
        a = parse();
        break;
      case KEY:
        a.value = u.value;
        a.branch.push_back(a);
        // v.push_back(a);
        break;
    }   
    a.branch.push_back(a);
    // std::cout << ast << ' ';
    std::cout << a << ' ';
  }
//   b.branch.push_back(branch);
  //   branch.push_back(b);
  return a;
}

void
AST::print() {
  static int level = 0;

  //   for (const auto& val : branch)
  for (int i = 0; i < branch.size(); i++) {
    // std::cout << leaf << ' ';
    // if (leaf == "{" & level++)
    //   std::cout << "|__ ";
    // if (leaf == ";")
    //   std::cout << std::endl;
    // std::cout << leaf << std::string(level * 3, ' ');
    // std::cout << std::endl;
  }
}

Parser::Parser(std::istream& s)
  : stream(s){};
