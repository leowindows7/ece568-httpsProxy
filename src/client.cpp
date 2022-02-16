#include "client.hpp"

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "network.hpp"
/** Connect to specified host
 * @param hostname: hostname to connect
 * @param port_num: port number
 * @param http_request
 * @return char * need to free by programmer
 **/
char * Client::connectToHost(std::string hostname,
                             int port_num,
                             std::string http_request) {
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

  if (send(socket_fd, http_request.c_str(), http_request.size(), 0) == -1) {
    // TOOO: refactor to throw exception
    perror("send");
    throw std::exception();
  }

  char recvbuffer = new char[65536];
  int numbytes = 0;

  if ((numbytes = recv(socket_fd, recvbuffer, 65536, 0)) == -1) {
    perror("recv");
    throw std::exception();
  }

  recvbuffer[numbytes] = '\0';

  std::string s(recvbuffer);
  if (s.find("\r\n") != std::string::npos) {
    std::cout << "client recv: " << recvbuffer << std::endl;
  }

  std::cout << "client recv: " << recvbuffer << std::endl;

  return recvbuffer;
}
