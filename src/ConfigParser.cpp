#include "ConfigParser.hpp"

#define DEBUG std::cout << "debbug\n";

std::ostream&
operator<<(std::ostream& o, const AST& rhs) {
  printAST(o, rhs, 0);
  return o;
}

/*
 * ------------------------------------| Get Token
 */
Token
ConfigParser::getToken() {
  std::string key;
  char c;
  stream >> std::ws;

  while (stream.good() && !std::isspace(stream.peek()) &&
         !std::isblank(stream.peek())) {
    switch (stream.peek()) {
      case TOKEN::SEMI:
      case TOKEN::LB:
      case TOKEN::RB:
        stream >> std::ws;
        if (stream.peek() == '\n') break;
        return (stream >> c, Token((TOKEN)c, std::string(1, c)));
      default:
        stream >> c;
        if (c == '\n') break;
        key.push_back(c);
        if (stream.peek() == ';' && key.length())
          return (Token(TOKEN::KEY, key));
    }
  }
  if (key.length()) return (Token(TOKEN::KEY, key));
  return Token();
}

/*
 * ------------------------------------| Parse Config
 */
AST
ConfigParser::createAST() {
  AST root;

  while (1) {
    AST node = parseConfig(1);
    if (node.value.empty()) break;
    root.branches.push_back(node);
    if (bracket) throw ConfigParser::Error("Error brackets");
  }
  return root;
};

AST
ConfigParser::parseConfig(int n) {
  AST tmp;
  AST node;

  Token token = getToken();
  switch (token.type) {
    case NONE:
      break;
    case TOKEN::SEMI:
      node = AST();
      break;
    case TOKEN::RB:
      node = AST();
      bracket--;
      break;
    case TOKEN::LB:
      bracket++;
      node = AST(token.value);
      while (!(tmp = parseConfig(1)).value.empty())
        node.branches.push_back(tmp);
      break;
    case TOKEN::KEY:
      node = AST(token.value);
      if (n) {
        while (tmp.value != "{" && !((tmp = parseConfig(0)).value.empty()))
          node.branches.push_back(tmp);
      }
      break;
    default:
      DEBUG;
      break;
  }

  return node;
}

/*
 * --------------------------------------------| Helpers
 */

void
printAST(std::ostream& o, const AST& a, int level) {
  std::string indent(level * 2, ' ');
  o << indent << "|__ " << a.value << '\n';
  for (auto& p : a.branches) printAST(o, p, level + 1);
}