#include "Config.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Server.hpp"
#include <arpa/inet.h>
#include <cstdint>
#include <cstdio>
#include <fcntl.h>
#include <fstream>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <variant>
#include <vector>

int
main(int ac, char** av) {
  //   s.create("localhost", 80)
  //     .on("/",
  //         [](Request const& req, Response& res) {
  //           std::string path = "public/";
  //           Server::servefiles(req, res, path);
  //         })
  //     .on("/503", [](Request const& req, Response& res) {
  //     Server::errorpage(req, res, 503); });
  //   s.run();

  if (ac != 2) return 1;
  std::fstream f(av[1]);

  // int main() {
  // std::ifstream f("config.conf");
  Config c(f);

  Server s(c);
  s.run();

  // Eventer ev;
  // ev.on<int>(
  //   "ev", [](Event<int>& ev) { std::cout << " 1 " << ev.data << std::endl;
  //   });
  // ev.on<int>(
  //   "ev", [](Event<int>& ev) { std::cout << " 2 " << ev.data << std::endl;
  //   });
  // int v = 20;
  // ev.emit<int>("ev", v);
  // test23();
}
