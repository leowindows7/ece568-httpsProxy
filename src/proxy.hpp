#ifndef PROXY_HPP
#define PROXY_HPP

#include "CacheController.hpp"
#include "client.hpp"
#include "server.hpp"

void sendToHost(std::string hostname, int port, int socketfd, std::string request);

// TODO: move this to Network class
void handleGetRequest(std::string hostname,
                      int port,
                      int socketfd,
                      std::string http_request,
                      std::map<std::string, std::string> & headerMap,
                      CacheController * cache);
// TODO: move this to Network class
void handleConnectRequest(std::string hostname,
                          int port,
                          int client_fd,
                          std::string http_request);
// TODO: move this to Network class
void setupConnectIO(struct pollfd pfds[], int client_fd, int server_fd);
void handlePostRequest(std::string hostname,
                       int port,
                       int client_fd,
                       std::string http_request);
void handleNewTab(int client_connection_fd, CacheController * cache);

class Proxy : public Server {
  CacheController * cache;

 public:
  Proxy() : cache(new CacheController(5)) {}
  Proxy(size_t size) : cache(new CacheController(size)) {}
  ~Proxy();
  int proxyServerSetup(int port);
  void serverBoot(int socketfd);
  void dispatch_worker(int socketfd, CacheController * cache);
};

#endif
