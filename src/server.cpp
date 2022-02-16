#include "server.hpp"

#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "inputParser.hpp"
#include "network.hpp"

/**
 * Constructor for scoket
 */
Server::Server() : connect(new Network()) {
}

/**
 * Setup socket for the server on specified port number
 * @param port_num: the port for socket to bind on
 **/
int Server::setupServer(const int port_num) {
  std::pair<int, struct addrinfo *> socketInfo =
      connect.get()->connectSetup<int, struct addrinfo *>(NULL, port_num);

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

  return socket_fd;
}

/**
 * Bootup Server on a given socket
 * @param socket_fd: socket field number
 **/

void Server::serverBoot(int socket_fd) {
  while (1) {
    // accept
    int client_connection_fd = acceptRequest(socket_fd);
    // receive message
    std::string buf = processRequest(client_connection_fd);
    std::cout << buf << std::endl;

    // send response
    char msg[] = "greeting from server\n";
    if (send(client_connection_fd, msg, strlen(msg), 0) == -1) {
      perror("send");
      exit(EXIT_FAILURE);
    }
  }
}

int Server::acceptRequest(int socket_fd) {
  struct sockaddr_storage socket_addr;
  socklen_t socket_addr_len = sizeof(socket_addr);
  int client_connection_fd = 0;
  client_connection_fd =
      accept(socket_fd, (struct sockaddr *)&socket_addr, &socket_addr_len);
  if (client_connection_fd == -1) {
    perror("accept");
    exit(EXIT_FAILURE);
  }

  return client_connection_fd;
}

std::string processRequest(int connection_fd) {
  char buf[512] = {0};
  int num_bytes = 0;
  if ((num_bytes = recv(connection_fd, buf, sizeof(buf), 0)) == -1) {
    perror("recv");
    exit(EXIT_FAILURE);
  }
  buf[num_bytes] = '\0';

  return std::string(buf);
}

/** 
 * Destructor for server class
 **/
Server::~Server() {
}
