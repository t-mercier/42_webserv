#include "Config.hpp"

#include <clocale>
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <ostream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>

#include "ConfigLocation.hpp"
#include "ConfigServer.hpp"

/*
 * --------------------------------------------< MACROS >
 */
#define PUSH(x)                                                                \
  for (const auto& b : a.branches) l.x.emplace_back(b.value);

#define ADD(x) l.x = a.branches[0].value;

#define PAIR(x)                                                                \
  l.x.first = std::stoi(a.branches[0].value);                                  \
  l.x.second = a.branches[1].value;

#define LINK(x, y)                                                             \
  for (size_t i = 0; i < a.branches.size() - 1; i++) {                         \
    const auto& b = a.branches[i];                                             \
    if (!b.value.empty() && !a.branches.back().value.empty())                  \
      l.x[y] = a.branches.back().value;                                        \
  }

/*
 * --------------------------------------------< Overload >
 */

std::ostream&
operator<<(std::ostream& o, const Config& rhs) {
  for (const auto& server : rhs.servers) o << server;
  return o;
}

std::ostream&
operator<<(std::ostream& o, const ConfigServer& rhs) {
  o << "[ Server ]\n";
  printServer(o, rhs);
  return o;
};

std::ostream&
operator<<(std::ostream& o, const ConfigLocation& rhs) {
  printLocation(o, rhs);
  return o;
}

/*
 * --------------------------------------------< Location >
 */

void
Config::createLocation(const AST& ast, ConfigLocation& l) {
  const auto lmap = l.getMap();
  for (const auto& a : ast.branches) {
    auto it = lmap.find(a.value);
    if (it == lmap.end()) continue;
    const auto& name = it->first;
    const auto& pair = it->second;
    if (it->second.second) continue;
    LOCATION KEY = pair.first;
    l.setMapValue(name, 1);
    switch (KEY) {
      case LOCATION::AUTO:
        l.autoindex = (a.branches[0].value == "on") ? true : false;
        break;
      case LOCATION::INDEX:
        PUSH(index);
        break;
      case LOCATION::METHODS:
        PUSH(methods);
        break;
      case LOCATION::RETURN:
        PAIR(return_);
        break;
      case LOCATION::CGI:
        LINK(cgi, b.value);
        break;
      case LOCATION::ROOT:
        ADD(root);
        break;
      case LOCATION::ERRPAGE:
        LINK(errPage, std::stoi(b.value));
        break;
      case LOCATION::LIMIT:
        l.limit = std::stoul(a.branches[0].value);
        break;
      default:
        break;
    }
  }
}

/*
 * --------------------------------------------< Server >
 */
ConfigServer
Config::createServer(const AST& serverAST) {
  ConfigServer server{};

  const auto map = server.getMap();
  for (const auto& a : serverAST.branches) {
    auto it = map.find(a.value);
    if (it == map.end()) continue;
    const auto& name = it->first;
    const auto& pair = it->second;
    server.setMapValue(name, 1);
    switch (pair.first) {
      case SERVER::NAME:
        for (const auto& b : a.branches) server.names.emplace_back(b.value);
        break;
      case SERVER::LISTEN: {
        auto x = a.branches[0].value;
        size_t pos = 0;
        if ((pos = x.find(':', 0)) != std::string::npos)
          server.listen[x.substr(0, pos)] = stoul(x.substr(pos + 1));
        else if ((pos = x.find('.', 0)) != std::string::npos)
          server.listen[x] = 8080;
        else server.listen["0.0.0.0"] = std::stoul(x, &pos, 10);
      } break;
      case SERVER::LOCATION: {
        std::string path;
        path = a.branches.empty() ? "/" : a.branches[0].value;
        if (a.branches.size() > 1) {
          ConfigLocation l;
          createLocation(a.branches[1], l);
          createLocation(serverAST, l);
          server.setLocation(path, l);
        }
      } break;
      default:
        break;
    }
  }

  if (!server.locations.count("/")) {
    ConfigLocation l;
    createLocation(serverAST, l);
    server.setLocation("/", l);
  }
  return server;
}

/*
 * --------------------------------------------< Config >
 */
void
Config::createConfig(std::istream& file) {
  ConfigParser p(file);
  auto ast = p.createAST();

  for (const auto& a : ast.branches) {
    if (a.value == "server") {
      auto server = createServer(a.branches[0]);
      servers.push_back(server);
    }
  };
}

/*
 * --------------------------------------------< Printers >
 */

void
printServer(std::ostream& o, const ConfigServer& s) {
  const auto& map = s.getMap();
  std::unordered_map<SERVER, std::function<void()>> printers = {
    { SERVER::NAME, [&s, &o]() { print(o, "[ server_name ]", s.names); } },
    { SERVER::LISTEN, [&s, &o]() { print(o, "[ listen ]", s.listen); } },
    { SERVER::ROOT, [&s, &o]() { print(o, "[ root ]", s.root); } },
    { SERVER::ERRPAGE, [&s, &o]() { print(o, "[ error_page ]", s.errPage); } },
    { SERVER::LOCATION, [&s, &o]() { print(o, "[ location ]", s.locations); } },
  };

  for (const auto& [key, pair] : map) {
    const auto& value = pair.second;
    if (!value) continue;
    if (printers.count(pair.first)) { printers[pair.first](); }
  }
}

void
printLocation(std::ostream& o, const ConfigLocation& l) {
  const auto& map = l.getMap();
  std::string i(4, ' ');
  std::unordered_map<LOCATION, std::function<void()>> printers = {
    { LOCATION::LIMIT,
      [&l, &o]() { print(o, "client_max_body_size", l.limit); } },
    { LOCATION::AUTO, [&l, &o]() { print(o, "autoindex", l.autoindex); } },
    { LOCATION::INDEX, [&l, &o]() { print(o, "index", l.index); } },
    { LOCATION::METHODS, [&l, &o]() { print(o, "allow_methods", l.methods); } },
    { LOCATION::RETURN, [&l, &o]() { print(o, "return", l.return_); } },
    { LOCATION::ROOT, [&l, &o]() { print(o, "root ", l.root); } },
    { LOCATION::ERRPAGE, [&l, &o]() { print(o, "error_page", l.errPage); } },
    { LOCATION::CGI, [&l, &o]() { print(o, "cgi", l.cgi); } },
  };

  for (const auto& [key, pair] : map) {
    const auto& value = pair.second;
    if (!value) continue;
    if (printers.count(pair.first)) { printers[pair.first](); }
  }
}