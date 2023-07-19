#include "HTTPServer.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <string>

#include "HTTPRequest.hpp"

#define BUFFER_SIZE 1024

HTTPServer::HTTPServer (){};

void
HTTPServer::create () {
  sock = socket (AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
    throw "socket";
  struct sockaddr_in addr;
  short port = 8080;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl (INADDR_ANY);
  addr.sin_port = htons (port);

  while (bind (sock, (struct sockaddr *)&addr, sizeof (addr))) {
    addr.sin_port = htons (++port);
  }

  printf ("bind localhost:%d\n", port);

  loop ();
}

void
HTTPServer::loop () {
  if (listen (sock, 10) < 0)
    throw "listen";

  struct sockaddr_in addr;
  socklen_t addrL = sizeof (addr);
  while (1) {
    int csock = accept (sock, (struct sockaddr *)&addr, &addrL);
    if (csock < 0)
      throw "accept";

    handle (csock);
  }
}

const char response[] = "HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/html\r\n"
                        "Connection: close\r\n"
                        "\r\n"
                        "<!DOCTYPE html>\r\n"
                        "<html>\r\n"
                        "<head>\r\n"
                        "    <title>Example Page</title>\r\n"
                        "</head>\r\n"
                        "<body>\r\n"
                        "    <h1>Hello, WORLD!</h1>\r\n"
                        "    <p>This is an example page.</p>\r\n"
                        "</body>\r\n"
                        "</html>\r\n";

#define AL(x) (sizeof (x) / sizeof (x[0]))

void
HTTPServer::handle (int csock) {
  std::string line;
  std::string rs;
  static char buf[BUFFER_SIZE];
  std::cout << "handling " << csock << std::endl;
  while (read (csock, buf, BUFFER_SIZE) > 0) {
    rs.append (buf);
    if (rs.find ("\r\n\r\n"))
      break;
  }

  HTTPRequest req (rs);

  std::cout << req << std::endl;
  write (csock, response, strlen (response));
  close (csock);
}

void
HTTPServer::run () {
  try {
    create ();
  } catch (char *s) {
    std::cout << s << std::endl;
  }
}