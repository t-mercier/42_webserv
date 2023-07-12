#include "HTTPServer.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <istream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

HTTPServer::HTTPServer(){};

void HTTPServer::create() {
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
    throw "socket";
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(8080);

  if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)))
    throw "bind";
  loop();
}

void HTTPServer::loop() {

  if (listen(sock, 10) < 0)
    throw "listen";

  struct sockaddr_in addr;
  socklen_t addrL = sizeof(addr);
  while (1) {
    int csock = accept(sock, (struct sockaddr *)&addr, &addrL);
    if (csock < 0)
      throw "accept";

    handle(csock);
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
                        "    <h1>Hello, Dicks!</h1>\r\n"
                        "    <p>This is an example page.</p>\r\n"
                        "</body>\r\n"
                        "</html>\r\n";
#define AL(x) (sizeof(x) / sizeof(x[0]))
void HTTPServer::handle(int csock) {
  std::string line;
  std::string req;
  static char buf[BUFFER_SIZE];
  std::cout << "handling " << csock << std::endl;
  while (read(csock, buf, BUFFER_SIZE) > 0) {
    req.append(buf);
    if (req.find("\r\n\r\n"))
      break;
  }

  std::cout << req << std::endl;
  send(csock, response, strlen(response), 0);
  close(csock);
}

void HTTPServer::run() { create(); }