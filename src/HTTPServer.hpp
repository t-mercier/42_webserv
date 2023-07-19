#pragma once
#include <sys/poll.h>

class HTTPServer {
public:
  HTTPServer ();
  void run ();
  void loop ();
  void create ();
  void handle (int);

private:
  int sock;
};
