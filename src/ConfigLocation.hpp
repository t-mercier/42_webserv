#pragma once
#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <ostream>
#include <string>
#include <type_traits>
#include <vector>

#include "ConfigParser.hpp"

/*
 * --------------------------------------------| ENUM
 */
enum class LOCATION {
  AUTO,
  INDEX,
  METHODS,
  ROOT,
  LIMIT,
  CGI,
  RETURN,
  ERRPAGE,
};

/*
 * --------------------------------------------| Configuration Location
 */
class ConfigLocation {
public:
  ConfigLocation() {
    autoindex = true;
    limit = 0;
  };
  ~ConfigLocation() = default;

  bool autoindex;
  std::vector<std::string> index;
  std::vector<std::string> methods;
  std::string root;
  std::map<std::string, std::string> cgi;
  uint32_t limit;
  std::pair<uint32_t, std::string> return_;
  std::map<uint32_t, std::string> errPage;

  std::map<std::string, std::pair<LOCATION, int>> getMap() const { return
  map; }

  void setMapValue(const std::string& X, int n) { this->map[X].second = n; }

private:
  std::map<std::string, std::pair<LOCATION, int>> map = {
    { "autoindex", { LOCATION::AUTO, 0 } },
    { "index", { LOCATION::INDEX, 0 } },
    { "allow_methods", { LOCATION::METHODS, 0 } },
    { "root", { LOCATION::ROOT, 0 } },
    { "cgi", { LOCATION::CGI, 0 } },
    { "error_page", { LOCATION::ERRPAGE, 0 } },
    { "client_max_body_size", { LOCATION::LIMIT, 0 } },
    { "return", { LOCATION::RETURN, 0 } }
  };
};


/*
 * --------------------------------------------| Helpers
 */
