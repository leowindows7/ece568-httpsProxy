#include "server.hpp"

#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "inputParser.hpp"
#include "network.hpp"

void Server::setupServer(const int port_num) {
  Network * connect = new Network();
  std::pair<int, struct addrinfo *> socketInfo =
      connect->connectSetup<int, struct addrinfo *>(NULL, port_num);

  int socket_fd = socketInfo.first;
  struct addrinfo * serviceinfo = socketInfo.second;

  int yes = 1;
  if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
    // TODO: throw exception
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  if (bind(socket_fd, serviceinfo->ai_addr, serviceinfo->ai_addrlen) == -1) {
    // TODO: throw exception
    perror("bind");
    exit(EXIT_FAILURE);
  }

  if (listen(socket_fd, 100) == -1) {
    // TODO: refactor this
    perror("listen");
    exit(EXIT_FAILURE);
  }

  while (1) {
    sleep(1);
    struct sockaddr_storage socket_addr;
    socklen_t socket_addr_len = sizeof(socket_addr);
    int client_connection_fd;
    client_connection_fd =
        accept(socket_fd, (struct sockaddr *)&socket_addr, &socket_addr_len);
    if (client_connection_fd == -1) {
      perror("accept");
      exit(EXIT_FAILURE);
    }

    char buf[512] = {0};
    int num_bytes = 0;
    if ((num_bytes = recv(client_connection_fd, buf, sizeof(buf), 0)) == -1) {
      perror("recv");
      exit(EXIT_FAILURE);
    }

    buf[num_bytes] = '\0';
    std::cout << buf << std::endl;

    if (send(client_connection_fd, buf, sizeof(buf), 0) == -1) {
      perror("send");
      exit(EXIT_FAILURE);
    }
  }

  freeaddrinfo(serviceinfo);
  if (close(socket_fd) == -1) {
    // TODO: throw exception
    perror("close");
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char ** argv) {
  std::vector<std::string> opts = {"port"};
  InputParser parser;
  std::unordered_map<std::string, int> parsedOpt = parser.parseOpt<int>(argc, argv, opts);

  Server server;
  server.setupServer(parsedOpt["port"]);
}
