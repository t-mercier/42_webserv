#pragma once
#include <exception>
#include <fstream>
#include <type_traits>

#include "ConfigLocation.hpp"
#include "ConfigParser.hpp"
#include "ConfigServer.hpp"

/*
 * --------------------------------------------< Configuration >
 */
class Config {
public:
  Config() = default;
  Config(std::istream& f) { createConfig(f); }
  ~Config() = default;

  std::vector<ConfigServer> servers;

  /*
   ! ------------------------------------------< Private
   */
private:
  void createConfig(std::istream&);
  void createLocation(const AST&, ConfigLocation &);
  void updateLocation(const AST&, ConfigLocation& l);
  ConfigServer createServer(const AST&);
  /*
   ! Error
   */
  class Error : public std::runtime_error {
  public:
    explicit Error(const char* what) : std::runtime_error(what) {}
  };
};

/*
 * --------------------------------------------< Overload >
 */
std::ostream&
operator<<(std::ostream& o, const ConfigServer& rhs);
std::ostream&
operator<<(std::ostream& o, const ConfigLocation& rhs);
std::ostream&
operator<<(std::ostream& o, const Config& rhs);

/*
 * --------------------------------------------< Printer >
 */
void
printLocation(std::ostream&, const ConfigLocation&);
void
printServer(std::ostream&, const ConfigServer&);

/*
 * --------------------------------------------< Template >
 */
template<typename T>
void
print(std::ostream& o, const std::string& s, const T& v) {
  using namespace std;

  string i(4, ' ');

  if (s.find('[') != std::string::npos) i = "";
  if constexpr ((is_same_v<T, string>) || (is_same_v<T, uint32_t>))
    o << i << s << ' ' << v << '\n';
  else if constexpr ((is_same_v<T, bool>))
    v ? o << i << s << " on\n" : o << i << s << " off\n";
  else if constexpr (is_same_v<T, vector<string>>)
    for (const auto& x : v) o << i << s + ' ' << x << '\n';
  else if constexpr (is_same_v<T, pair<string, string>>)
    o << i << s + ' ' << v.first << ' ' << v.second << '\n';
  else if constexpr (is_same_v<T, pair<uint32_t, string>>)
    o << i << s + ' ' << v.first << ' ' << v.second << '\n';
  else if constexpr (is_same_v<T, map<string, string>>)
    for (const auto& [x, y] : v) o << i << s << ' ' << x << ' ' << y << '\n';
  else if constexpr (is_same_v<T, map<uint32_t, string>>)
    for (const auto& [x, y] : v) o << i << s << ' ' << x << ' ' << y << '\n';
  else if constexpr (is_same_v<T, map<string, uint32_t>>)
    for (const auto& [x, y] : v) o << i << s << ' ' << x << ':' << y << '\n';
  else if constexpr (is_same_v<T, map<string, ConfigLocation>>)
    for (const auto& [x, y] : v) o << i << s << ' ' << x << '\n' << y << '\n';
}
