#ifndef PROXY_HPP
#define PROXY_HPP

#include "client.hpp"
#include "server.hpp"

void sendToHost(std::string hostname, int port, int socketfd, std::string request);

class Proxy : public Server {
 public:
  int proxyServerSetup(int port);
  void serverBoot(int socketfd);
  void dispatch_worker(std::string hostname, int port, int socketfd, std::string buf);
};

#endif
