#include "HTTPServer.hpp"
#include <sys/poll.h>

HTTPServer::HTTPServer(){};

void run() {
  while (true) {
    int ready_descriptors = poll();
    if (ready_descriptors < 0) {
      //... handle error
    } else if (ready_descriptors == 0) {
      //... handle timeout
    } else {
      //... handle ready descriptors
    }
  }
}