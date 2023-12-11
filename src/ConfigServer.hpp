#pragma once
#include <sstream>
#include <string>

#include "ConfigLocation.hpp"

/*
 * --------------------------------------------| ENUM Server
 */
enum class SERVER {
  NAME,
  LISTEN,
  ROOT,
  ERRPAGE,
  LOCATION,
};

/*
 * --------------------------------------------| Configuration Server
 */

class ConfigServer {
public:
  ConfigServer() = default;
  ~ConfigServer() = default;

  std::vector<std::string> names;
  std::map<std::string, uint32_t> listen;
  std::string root;
  std::map<uint32_t, std::string> errPage;
  std::map<std::string, ConfigLocation> locations;

  std::map<std::string, std::pair<SERVER, int>> getMap() const { return map; }
  void setMapValue(const std::string& X, int n) { this->map[X].second = n; }

  void setLocation(const std::string& path, const ConfigLocation& location) {
    setMapValue("location", 1);
    locations[path] = location;
  }

private:
  std::map<std::string, std::pair<SERVER, int>> map = {
    { "server_name", { SERVER::NAME, 0 } },
    { "listen", { SERVER::LISTEN, 0 } },
    { "root", { SERVER::ROOT, 0 } },
    { "error_page", { SERVER::ERRPAGE, 0 } },
    { "location", { SERVER::LOCATION, 0 } }
  };
};
