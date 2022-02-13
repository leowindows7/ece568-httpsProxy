#ifndef PROXY_HPP
#define PROXY_HPP

#include "client.hpp"
#include "server.hpp"

class Proxy : public Server {
 public:
  int proxyServerSetup(int port);
  void serverBoot(int socketfd);
};

#endif
