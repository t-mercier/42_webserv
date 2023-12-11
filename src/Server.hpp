#pragma once
#include <arpa/inet.h>
#include <dirent.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/errno.h>
#include <sys/poll.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <algorithm>
#include <cerrno>
#include <csignal>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "Config.hpp"
#include "ConfigLocation.hpp"
#include "ConfigServer.hpp"
#include "File.hpp"
#include "Request.hpp"
#include "Response.hpp"

#ifdef LINUX
#include <sys/epoll.h>
#endif

/* for macOS compliance */
extern char** environ;

struct CompareByLength {
  bool operator()(const std::string& a, const std::string& b) const {
    return a.size() > b.size();
  }
};

class VirtualServer {
 public:
  VirtualServer(ConfigServer const& config);
  void handle(Request const& req, Response& res);
  VirtualServer& on(std::string const& path, f_handler f);
  bool hasName(std::string const&);

 private:
  std::map<std::string, f_handler, CompareByLength> handlers;
  ConfigServer const& config;
};

class Server {
 public:
  static void servefiles(Request const& req, Response& res, std::string& path);
  static void errorpage(Response& res, int code);
  static void autoindex(Request const& req, Response& res, std::string path);
  static void cgi(Request const& req,
                  Response& res,
                  std::string exepath,
                  std::string filepath);
  static void sendfile(Request const& req, Response& res, std::string path);
  void handle(Request const& req,
              Response& res,
              std::string const& name,
              uint16_t port);
  Server();
  ~Server();
  Server(Config const& cfg);

  void run();
  ServerSocket& create(std::string name, uint16_t port);
  void setpfd(File* s, uint32_t events);

 private:
  void loop();
  void remove(int fd, int i);
  std::map<std::string, std::map<int, std::vector<VirtualServer*>>> npvssm;
  std::map<std::string, std::map<int, ServerSocket*>> npssm;
  std::map<int, File*> files;
  std::vector<struct pollfd> pfds;
  std::vector<int> toremove;
};
