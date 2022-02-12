#include "client.hpp"

#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "inputParser.hpp"
#include "network.hpp"

void Client::connectToHost(std::string hostname, int port_num) {
  Network * client_connect = new Network();
  std::pair<int, struct addrinfo *> socketInfo =
      client_connect->connectSetup<int, struct addrinfo *>(hostname.c_str(), port_num);

  int socket_fd = socketInfo.first;
  struct addrinfo * serviceinfo = socketInfo.second;

  if (connect(socket_fd, serviceinfo->ai_addr, serviceinfo->ai_addrlen) == -1) {
    // TOOO: refactor to throw exception
    perror("connect");
    exit(EXIT_FAILURE);
  }

  char msg[] = "CONNECT www.google.com:443 HTTP/1.1 \
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:95.0) Gecko/20100101 Firefox/95.0 \
Proxy-Connection: keep-alive \
Connection: keep-alive \
Host: www.google.com:443";
  if (send(socket_fd, msg, strlen(msg), 0) == -1) {
    // TOOO: refactor to throw exception
    perror("send");
    exit(EXIT_FAILURE);
  }

  char recvbuffer[512] = {0};
  if (recv(socket_fd, recvbuffer, sizeof(recvbuffer), 0) == -1) {
    perror("recv");
    exit(EXIT_FAILURE);
  }

  std::cout << recvbuffer << std::endl;

  freeaddrinfo(serviceinfo);
}

int main(int argc, char ** argv) {
  std::vector<std::string> opts = {"hostname", "port"};
  InputParser parser;
  std::unordered_map<std::string, std::string> parsedOpt =
      parser.parseOpt<std::string>(argc, argv, opts);
  for (auto iter = parsedOpt.begin(); iter != parsedOpt.end(); iter++) {
    std::cout << iter->first << ": " << iter->second << std::endl;
  }

  Client client;
  std::string hostname = parsedOpt["hostname"];
  int port = std::stoi(parsedOpt["port"]);
  client.connectToHost(hostname, port);
}
