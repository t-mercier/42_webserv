#pragma once
#include <sys/poll.h>

class HTTPServer {
public:
  HTTPServer();
  void run();
  void accept();

private:
};
