#include "client.hpp"

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "network.hpp"

char * Client::connectToHost(std::string hostname, int port_num) {
  Network * client_connect = cconnect.get();
  std::pair<int, struct addrinfo *> socketInfo =
      client_connect->connectSetup<int, struct addrinfo *>(hostname.c_str(), port_num);

  int socket_fd = socketInfo.first;
  struct addrinfo * serviceinfo = socketInfo.second;

  if (connect(socket_fd, serviceinfo->ai_addr, serviceinfo->ai_addrlen) == -1) {
    // TOOO: refactor to throw exception
    perror("connect");
    throw std::exception();
  }

  std::string http_request = "GET / HTTP/1.1\nHost:" + hostname + "\n\n";
  if (send(socket_fd, http_request.c_str(), http_request.size(), 0) == -1) {
    // TOOO: refactor to throw exception
    perror("send");
    throw std::exception();
  }

  char * recvbuffer = new char[65536];
  if (recv(socket_fd, recvbuffer, sizeof(recvbuffer), 0) == -1) {
    perror("recv");
    throw std::exception();
  }

  // std::cout << recvbuffer << std::endl;

  return recvbuffer;
}
