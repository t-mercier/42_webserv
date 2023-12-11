#include "File.hpp"
#include "Server.hpp"

ClientSocket::ClientSocket(int fd, Server& s, f_handler h)
  : File(fd),
    _state(0),
    offset(0),
    handle(h),
    want_length(-1),
    have_length(0),
    req(stream) {
  req.server = &s;
}

File::~File() {
  int c = close(fd);
  if (c < 0)
    perror("close");
}

void ClientSocket::read() {
  static char b[BUFFER_SIZE];
  ssize_t n = ::read(fd, b, BUFFER_SIZE);
  if (n > 0) {
    stream.write(std::string(b, n));
    have_length += n;
  } else if (n < 0) {
    done = true;
    return;
  } else if (n == 0) {
  }
  size_t p;
  if (_state == 0 && (p = stream.find("\r\n\r\n")) != stream.npos) {
    req.parse_head();
    want_length = std::stol(req.headers["content-length"]);
    have_length -= p + 4;
    _state = 1;
  }
  if (_state == 1 && have_length >= want_length)
    _state = 2;
}

void ClientSocket::write() {
  if (_state == 2) {
    req.parse_body();
    TRACE() << req.body.size() << std::endl;
    handle(req, res);

    _state = 3;
  }
  if (_state == 3 && res.done()) {
    std::string& p = res.final();
    ssize_t q = ::write(fd, p.c_str() + offset, p.size() - offset);
    TRACE() << "wrote " << q << " to " << fd << std::endl;
    if (q > 0)
      offset += q;
    if (q < 0) {
      hangup();
      return;
    }
    if (q == 0) {
    }
    TRACE() << q << " " << offset << "/" << p.size() << std::endl;
    if (offset == p.size())
      done = true;
  }
}

void ClientSocket::hangup() {
  done = true;
}

ServerSocket::ServerSocket(Server& s, std::string name, uint16_t port)
  : File(socket(AF_INET, SOCK_STREAM, 0)), name(name), port(port), server(s) {
  if (fd < 0)
    throw std::runtime_error("socket");
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  inet_pton(AF_INET, name.c_str(), &addr.sin_addr);
  addr.sin_port = htons(port);

  if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)))
    throw std::runtime_error("bind");

  if (listen(fd, 10) < 0)
    throw std::runtime_error("listen");

  std::cout << "OPEN " << name.c_str() << ":" << port << std::endl;

  server.setpfd(this, POLLIN);
}

void ServerSocket::read() {
  int sock = ::accept(fd, 0, 0);
  if (sock == -1) {
    perror("accept");
    std::cerr << fd << " " << errno << std::endl;
    return;
  }
  TRACE() << "accept " << fd << " -> " << sock << std::endl;
  int flags = fcntl(sock, F_GETFL, 0);
  TRACE() << "flag " << flags << std::endl;
  fcntl(sock, F_SETFL, flags | O_NONBLOCK);

  auto cs =
    new ClientSocket(sock, server, [this](Request const& req, Response& res) {
      server.handle(req, res, name, port);
    });

  server.setpfd(cs, POLLIN | POLLOUT);
}

void CGIWriter::write() {
  size_t s = req.body.size();
  if (q < s) {
    ssize_t n = ::write(fd, req.body.data() + q, req.body.size() - q);
    TRACE() << "writer write " << n << " " << req.body.size() << std::endl;
    if (n > 0)
      q += n;
    else {
    }
  } else {
    TRACE() << "writer done" << std::endl;

    done = true;
  }
}

void CGIReader::read() {
  static char b[BUFFER_SIZE];
  ssize_t n;
  (n = ::read(fd, b, BUFFER_SIZE));
  if (n > 0) {
    TRACE() << "reader read " << n << std::endl;
    buf.write(std::string(b, n));
  } else
    hangup();
}

void CGIReader::onTimeout() {
  kill(pid, SIGKILL);
  res.error(504);
  done = true;
}

void CGIReader::hangup() {
  TRACE() << "reader done" << std::endl;
  // std::cout << buf << std::endl;
  int q = 0;
  while (!buf.eof()) {
    if (buf.peek(2) == "\r\n") {
      q = 1;
      break;
    }
    std::string k = buf.rword();
    std::string v = buf.rword();
    buf.rline();
    if (buf.find("\r") == buf.npos)
      break;
    res.header(k.substr(0, k.length() - 1), v);
  }

  if (!q) {
    res.error(502);
  } else {
    res.body() << buf.rrest();
    res.end();
  }

  done = true;
}