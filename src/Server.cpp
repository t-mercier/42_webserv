#include "Server.hpp"



Server::Server() = default;
Server::~Server() {
  for (auto& a : npvssm)
    for (auto& b : a.second)
      for (auto& c : b.second) delete c;
  for (auto& a : files) delete a.second;
};

VirtualServer::VirtualServer(ConfigServer const& config) : config(config) {}

bool
isPrefix(const std::string& a, const std::string& b) {
  if (a.size() > b.size()) return false;
  auto result = std::mismatch(a.begin(), a.end(), b.begin());
  return result.first == a.end();
}

bool
isSuffix(const std::string& str, const std::string& suffix) {
  return str.size() >= suffix.size() &&
         0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
}

class LocationHandler {
public:
  LocationHandler(std::string const& path, ConfigLocation const& c)
    : config(c), path(path) {}

  void errorpage(Request const& req, Response& res, int code) {
    auto it = config.errPage.find(code);
    if (it != config.errPage.end()) {
      res.code(code);
      std::filesystem::path fpath = config.root;
      fpath /= "./" + it->second;
      fpath = fpath.lexically_normal();
      Server::sendfile(req, res, fpath);
      return;
    }
    Server::errorpage(res, code);
  }

  void operator()(Request const& req, Response& res) {
    res.setErrorF(
      [req, this](Response& res, int code) { errorpage(req, res, code); });

    if (config.limit != 0 && config.limit < req.body.size()) {
      return res.error(413);
    }

    if (!config.methods.empty()) {
      auto it =
        std::find(config.methods.begin(), config.methods.end(), req.method);
      if (it == config.methods.end()) return res.error(405);
    }

    std::filesystem::path fpath = config.root;
    fpath /= "./" + req.path;
    fpath = fpath.lexically_normal();

    if (!config.return_.second.empty()) {
      res.begin();
      res.code(config.return_.first);
      res.header("Location", config.return_.second);
      res.end();
      return;
    }

    for (auto& p : config.cgi) {
      if (isSuffix(req.path, p.first))
        return Server::cgi(req, res, p.second, fpath);
    }

    if (!config.root.empty()) {
      TRACE() << "path? " << fpath << std::endl;
      if (std::filesystem::is_directory(fpath)) {
        for (auto& idx : config.index) {
          Server::sendfile(req, res, fpath / idx);
          if (res.isHandled()) return;
        }
        if (config.autoindex) Server::autoindex(req, res, fpath);
        return;
      }
      Server::sendfile(req, res, fpath);
      return;
    }
  }

private:
  ConfigLocation const& config;
  std::string path;
};

Server::Server(Config const& c) : Server() {
  for (auto& server : c.servers) {
    VirtualServer* vs = new VirtualServer(server);
    for (auto& location : server.locations) {
      vs->on(location.first, LocationHandler(location.first, location.second));
    }
    for (auto& address : server.listen) {
      create(address.first, address.second);
      npvssm[address.first][address.second].push_back(vs);
    }
  }
}

ServerSocket&
Server::create(std::string name, uint16_t port) {
  if (npssm[name][port]) return *npssm[name][port];
  return *(npssm[name][port] = new ServerSocket(*this, name, port));
}

void
Server::setpfd(File* s, uint32_t events) {
  int fd = s->getfd();
  if (files.count(fd)) {
    for (auto& pfd : pfds) {
      if (pfd.fd == fd) pfd.events = events;
    }
  } else {
    struct pollfd pfd = {};
    TRACE() << "pfd adding " << fd << " " << s << std::endl;
    files[fd] = s;
    pfd.fd = fd;
    pfd.events = events;
    pfds.push_back(pfd);
  }
}

void
Server::remove(int fd, int i) {
  TRACE() << "removing " << fd << std::endl;
  toremove.push_back(i);
  delete files[fd];
  files.erase(fd);
}

void
Server::loop() {
  while (1) {
    (void)poll(pfds.data(), pfds.size(), 1000);
    for (size_t i = 0; i < pfds.size(); i++) {
      struct pollfd pfd = pfds[i];
      int fd = pfd.fd;
      files[fd]->timeout();
      if (!files[fd]->isDone() && pfd.revents & POLLIN) files[fd]->read();
      if (!files[fd]->isDone() && pfd.revents & POLLOUT) files[fd]->write();
      if (!files[fd]->isDone() && pfd.revents & POLLHUP) files[fd]->hangup();
      if (files[fd]->isDone()) remove(fd, i);
    }
    std::sort(toremove.begin(), toremove.end(), std::greater<int>());
    for (auto& i : toremove) pfds.erase(pfds.begin() + i);
    toremove.clear();
  }
}

std::string
prelude() {
  return "<style>"
         "body{height:100vh;margin:0;background:#040204;color:white;font-"
         "family:"
         "Courier;}"
         ".err{color:red;}"
         ".wh100{width:100%;height:100%;}"
         "h1,h2{font-weight:500;}"
         "h2{font-size:2em;}"
         ".c{display:flex;flex-direction:column;}"
         ".r{display:flex;}"
         ".cc{justify-content:center;align-items:center;}"
         "a{color:cyan;}"
         ".p1{padding:1rem;}"
         ".s{font-size:1.4em;}"
         "</style>";
}

void
Server::errorpage(Response& res, int code) {
  res.begin();
  res.code(code);
  res.header("Content-Type", Response::MIME.at("html"));
  res.body() << prelude();
  res.body() << "<div class='wh100 cc c'><img width=100 "
                "src='https://i.ibb.co/zhBdVyK/skull.gif'/><h2 "
                "class='err'>ERROR: "
             << code << "</h2><span class='s'>" << Response::STATUS[code]
             << "</span></div>\n";
  res.end();
}

std::string
toenv(std::string x) {
  std::string r;
  for (auto& e : x)
    if (e == '-') r.push_back('_');
    else r.push_back(toupper(e));
  return r;
}

void
Server::cgi(Request const& req,
            Response& res,
            std::string exepath,
            std::string filepath) {
  TRACE() << "cgi for " << exepath << " " << filepath << std::endl;

  char const* argv[] = { exepath.c_str(), filepath.c_str(), 0 };

  std::map<std::string, std::string> env;
  env["PATH_INFO"] = req.path;
  if (req.headers.count("content-type"))
    env["CONTENT_TYPE"] = req.headers.at("content-type");
  if (req.headers.count("content-length"))
    env["CONTENT_LENGTH"] = req.headers.at("content-length");
  env["SERVER_PROTOCOL"] = req.version;
  env["REQUEST_METHOD"] = req.method;
  std::string qs = req.queryString();
  env["QUERY_STRING"] = qs;
  env["SCRIPT_NAME"] = filepath;
  env["REDIRECT_STATUS"] = "200";

  for (auto& e : req.headers) env["HTTP_" + toenv(e.first)] = e.second;

  std::vector<std::string> temp;
  for (auto& p : env) temp.push_back(p.first + "=" + p.second);

  char** oenv = environ;
  while (*oenv) temp.push_back(*oenv++);

  const char* envp[temp.size() + 1];
  int i = 0;
  for (auto& e : temp) envp[i++] = e.c_str();
  envp[i] = 0;

  int fdi[2];
  int fdo[2];
  int a = pipe(fdi);
  if (a) throw std::runtime_error("cgi pipe");
  a = pipe(fdo);
  if (a) throw std::runtime_error("cgi pipe");

  int flags = fcntl(fdi[1], F_GETFL, 0);
  fcntl(fdi[1], F_SETFL, flags | O_NONBLOCK);
  flags = fcntl(fdo[0], F_GETFL, 0);
  fcntl(fdo[0], F_SETFL, flags | O_NONBLOCK);

  int pid = fork();

  if (!pid) {
    close(fdi[1]);
    dup2(fdi[0], 0);
    close(fdi[0]);

    close(fdo[0]);
    dup2(fdo[1], 1);
    close(fdo[1]);

    execve(argv[0],
           static_cast<char* const*>((void*)argv),
           static_cast<char* const*>((void*)envp));
    std::cout << "cant execve" << std::endl;
    exit(1);
  } else if (pid < 0) {
    throw std::runtime_error("fork");
  }

  close(fdi[0]);
  close(fdo[1]);

  res.begin();
  req.server->setpfd(new CGIWriter(fdi[1], req), POLLOUT);
  req.server->setpfd(new CGIReader(fdo[0], res, pid), POLLIN | POLLHUP);
  TRACE() << "end cg2i" << std::endl;
}

void
Server::autoindex(Request const& req, Response& res, std::string path) {
  // std::cout << "autoindex of " << path << std::endl;
  std::vector<std::string> result;
  DIR* dir = opendir(path.c_str());

  if (dir) {
    struct dirent* entry;
    while ((entry = readdir(dir)))
      result.push_back(std::string(entry->d_name) +
                       (entry->d_type == DT_DIR ? "/" : ""));
    closedir(dir);
  } else {
    return;
  }

  res.begin();
  res.header("Content-Type", Response::MIME.at("html"));
  res.body() << prelude();
  res.body() << "<div class='c p1'>";
  res.body() << "<h1>Index of " << req.path << "</h1>";
  for (auto& it : result)
    res.body() << "<a href='./" << it << "'> " << it << "</a>";
  res.body() << "</div>";
  res.end();
}

void
Server::sendfile(Request const& req, Response& res, std::string path) {
  (void)req;
  std::ifstream f(path);
  // std::cout << "trying " << path << std::endl;
  if (f.is_open()) {
    res.begin();
    for (auto& p : Response::MIME) {
      if (isSuffix(path, p.first)) {
        res.header("Content-Type", p.second);
        break;
      }
    }
    // std::cout << "sending " + path << std::endl;
    res.body() << f.rdbuf();
    res.end();
  }
}

void
Server::servefiles(Request const& req, Response& res, std::string& bpath) {
  std::string path = bpath + req.path;
  if (path.back() == '/') {
    std::string index = path + "index.html";
    sendfile(req, res, index);
    if (!res.isHandled()) autoindex(req, res, path);
    return;
  }
  sendfile(req, res, path);
}

void
Server::run() {
  try {
    loop();
  } catch (std::exception& s) { std::cout << s.what() << std::endl; }
}

void
Server::handle(Request const& req,
               Response& res,
               std::string const& name,
               uint16_t port) {
  std::cout << name << ":" << port << " " << req.method << " " << req.path
            << std::endl;
  if (req.headers.end() != req.headers.find("host")) {
    for (auto& vs : npvssm[name][port]) {
      std::string host = req.headers.at("host");
      auto i = host.find(':');
      if (i != host.npos) host = host.substr(0, i);
      bool found = vs->hasName(host);
      if (found) return vs->handle(req, res);
    }
  }
  return npvssm[name][port].front()->handle(req, res);
}

void
VirtualServer::handle(Request const& req, Response& res) {
  for (auto& it : handlers) {
    // std::cout << it.first << " " << req.path << std::endl;
    if (isPrefix(it.first, req.path)) {
      it.second(req, res);
      if (res.isHandled()) return;
    }
  }
  if (config.errPage.count(404)) {
    res.code(404);
    std::filesystem::path path = config.errPage.at(404);
    path = config.root / path;
    Server::sendfile(req, res, path);
  } else Server::errorpage(res, 404);
}

VirtualServer&
VirtualServer::on(std::string const& path, f_handler f) {
  handlers[path] = f;
  return *this;
}

bool
VirtualServer::hasName(std::string const& name) {
  auto it = std::find(config.names.begin(), config.names.end(), name);
  return it != config.names.end();
}