#pragma once
#include "Request.hpp"
#include "Response.hpp"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <csignal>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <ctime>
#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>

#ifdef LINUX
#include <asm-generic/errno-base.h>
#include <sys/epoll.h>
#endif

class Server;
using f_handler = std::function<void(Request const& req, Response& res)>;

#define BUFFER_SIZE (1 << 16)

class File {
 public:
  File(int fd, time_t timeout = 0)
    : fd(fd), done(false), _timeout(timeout), _start(time(0)) {}
  virtual ~File();
  virtual void read(){};
  virtual void write(){};
  virtual void hangup(){};
  virtual void onTimeout(){};
  bool isDone() const { return done; }
  int getfd() const { return fd; };
  bool timeout() {
    if (_timeout > 0 && time(0) - _start >= _timeout) {
      onTimeout();
      std::cerr << fd << " timeout" << std::endl;
      return 1;
    }
    return 0;
  }

 protected:
  int fd;
  bool done;
  time_t _timeout;
  time_t _start;
};

class ClientSocket : public File {
 public:
  ClientSocket(int fd, Server& s, f_handler h);
  void read();
  void write();
  void hangup();

 private:
  int _state;
  size_t offset;
  f_handler handle;
  size_t want_length;
  size_t have_length;
  Response res;
  Request req;
  Stream stream;
};

class ServerSocket : public File {
 public:
  ServerSocket(Server& s, std::string name, uint16_t port);
  void read();

 private:
  std::string name;
  uint16_t port;
  Server& server;
};

class CGIReader : public File {
 public:
  CGIReader(int fd, Response& res, int pid)
    : File(fd, 30), res(res), pid(pid){};
  void read();
  void onTimeout();
  void hangup();
  Response& res;
  Stream buf;
  int pid;
};

class CGIWriter : public File {
 public:
  CGIWriter(int fd, Request const& req) : File(fd), req(req), q(0){};
  void write();
  Request const& req;
  size_t q;
};