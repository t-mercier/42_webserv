#include "Parser.hpp"
#include <_ctype.h>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <istream>
#include <iterator>

/*_____________________________ constructors _____________________________*/

Token::Token() : id(TOKEN::NONE), value(""){};

Token::Token(TOKEN _id, std::string _v) : id(_id), value(_v){};

AST::AST() : branch(0), leaf(""){};

/*______________________________ destructor ______________________________*/

/*_______________________________ overloads ______________________________*/

// std::ostream& operator<<(std::ostream& o, AST& rhs) {
//   std::string indent(rhs.level * 2, ' ');
//   //   for (const auto& rhs : rhs.branch)
//   if (!rhs.leaf.empty())
//     o << indent << "|__ " << ' ' << rhs.leaf << std::endl;
//   return o;
// }

/*_______________________________ accessors ______________________________*/

// std::string Parser::getLeaf() {
//   return leaf;
// }

/*________________________________ methods _______________________________*/

Token Parser::getToken() {
  std::string key;
  char c;
  char b;
  stream >> std::ws;
  while (stream.good() && !std::isspace(stream.peek())) {
    switch (stream.peek()) {
      case SEMI:
        return (stream >> c, Token((TOKEN)c, std::string(1, c)));
      case LB:
        return (stream >> c, Token((TOKEN)c, std::string(1, c)));
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

AST Parser::parse() {
  Token u;
  AST root, a, b;

  while ((u = getToken()).id != NONE) {
    switch (u.id) {
      case NONE:
        break;
      case LB:
        a.branch.push_back(parse());
        root.branch.push_back(a);
        a = AST();
        break;
      case SEMI:
        root.branch.push_back(a);
        a = AST();
        break;
      case RB:
        return root;
      case KEY: {
        if (a.leaf.empty()) {
          a.leaf = u.value;
        } else {
          AST b;
          b.leaf = u.value;
          a.branch.push_back(b);
        }
        break;
      }
      default:
        break;
    }
  }
  return root;
}

void AST::print() const {
  printHelper(0);
}

void AST::printHelper(int level) const {
  std::string indent(level * 2, ' ');
  if (leaf != "")
    std::cout << indent << "|__ " << ' ' << leaf << std::endl;
  for (const auto& b : branch)
    b.printHelper(level + 1);
}

Parser::Parser(std::istream& s) : stream(s){};
