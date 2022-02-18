#ifndef PROXY_HPP
#define PROXY_HPP

#include "client.hpp"
#include "server.hpp"

void sendToHost(std::string hostname, int port, int socketfd, std::string request);

// TODO: move this to Network class
void handleGetRequest(std::string hostname,
                      int port,
                      int socketfd,
                      std::string http_request);
// TODO: move this to Network class
void handleConnectRequest(std::string hostname,
                          int port,
                          int client_fd,
                          std::string http_request);
// TODO: move this to Network class
void setupConnectIO(struct pollfd pfds[], int client_fd, int server_fd);

void handleNewTab(int client_connection_fd);

class Proxy : public Server {
 public:
  int proxyServerSetup(int port);
  void serverBoot(int socketfd);
  void dispatch_worker(int socketfd);
};

#endif
