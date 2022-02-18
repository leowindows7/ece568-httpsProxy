#include "proxy.hpp"

#include <poll.h>

#include <cstring>
#include <iostream>
#include <map>
#include <string>
#include <thread>

#include "httpParser.hpp"
#include "inputParser.hpp"
#include "network.hpp"

int Proxy::proxyServerSetup(int port) {
  return setupServer(port);
}

void Proxy::serverBoot(int socketfd) {
  while (1) {
    int client_connection_fd = acceptRequest(socketfd);
    dispatch_worker(client_connection_fd);
  }
}

// TODO: move this to network class
void handleGetRequest(std::string hostname,
                      int port,
                      int client_fd,
                      std::string http_request) {
  Client c;
  std::string recvbuf;
  int server_fd = c.setUpSocket(hostname, port);
  recvbuf = c.connectToHost(hostname, port, http_request, server_fd);

  if (send(client_fd, recvbuf.c_str(), recvbuf.size(), 0) == -1) {
    // TOOO: refactor to throw exception
    perror("send");
    throw std::exception();
  }
}

void handleNewTab(int client_connection_fd) {
  while (1) {
    // get header from client
    std::string http_request = Network::recvRequest(client_connection_fd);
    std::cout << "Request Header:" << std::endl
              << http_request << std::endl;  // TODO: remove this

    // parse header
    HttpParser httpParser;
    std::map<std::string, std::string> headerMap = httpParser.httpResMap(http_request);
    std::string hostname = headerMap["host"];
    int port = (headerMap["Port"] == "-1" || headerMap["Port"] == "")
                   ? 80
                   : std::stoi(headerMap["Port"]);
    std::string method = headerMap["Method"];

    // handle action
    if (method.find("GET") != std::string::npos) {
      handleGetRequest(hostname, port, client_connection_fd, http_request);
    }

    else if (method.find("CONNECT") != std::string::npos) {
      // handleConnectRequest(hostname, port, client_connection_fd, http_request);
      std::cout << "Fuck you" << std::endl;
    }
  }
}

void Proxy::dispatch_worker(int socketfd) {
  // std::cout << buf << std::endl;
  std::thread(handleNewTab, socketfd).detach();
}

// TODO: move this to network class
void handleConnectRequest(std::string hostname,
                          int port,
                          int client_fd,
                          std::string http_request) {
  // setup connection with server
  Client c;
  int server_fd = c.setUpSocket(hostname, port);
  // send back 200 ok with client
  HttpParser httpParser;
  std::string responseOk = httpParser.send200OK();
  Network::sendRequest(client_fd, responseOk.c_str(), responseOk.size());

  // setup IO mux
  struct pollfd pfds[2];  // only client and server
  int fd_size = 2;
  setupConnectIO(pfds, client_fd, server_fd);
  for (int i = 0; i < 2; i++) {
    std::cout << pfds[i].fd << std::endl;
  }
  // Use Poll IO to listen to socket that is ready for recving response
  while (1) {
    int poll_count = poll(pfds, fd_size, -1);
    std::string recvBuf;
    for (int i = 0; i < fd_size; i++) {
      if (pfds[i].revents & POLLIN) {
        recvBuf = Network::recvRequest(pfds[i].fd);
        int sendIndex = fd_size - i - 1;
        Network::sendRequest(pfds[sendIndex].fd, recvBuf.c_str(), recvBuf.size());
      }
    }
  }
}

// TODO: move this to Network class
void setupConnectIO(struct pollfd pfds[], int client_fd, int server_fd) {
  pfds[0].fd = client_fd;
  pfds[0].events = POLLIN;
  pfds[1].fd = server_fd;
  pfds[1].events = POLLIN;
}

int main(int argc, char ** argv) {
  std::vector<std::string> opts = {"port"};
  InputParser parser;
  std::unordered_map<std::string, int> parsedOpt = parser.parseOpt<int>(argc, argv, opts);

  Proxy p;
  int socketfd = p.proxyServerSetup(parsedOpt["port"]);
  p.serverBoot(socketfd);
}
