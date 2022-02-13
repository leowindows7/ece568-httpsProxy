#include "proxy.hpp"

#include <cstring>
#include <iostream>
#include <string>

#include "inputParser.hpp"

int Proxy::proxyServerSetup(int port) {
  return setupServer(port);
}

void Proxy::serverBoot(int socketfd) {
  while (1) {
    int client_connection_fd = acceptRequest(socketfd);
    std::string buf = processRequest(client_connection_fd);
    std::cout << "In Proxy:" << std::endl << buf << std::endl;
    // TODO: refactor this to use http parser
    Client c;
    char * recvbuf = c.connectToHost("vcm-24287.vm.duke.edu", 12345);
    if (send(client_connection_fd, recvbuf, strlen(recvbuf), 0) == -1) {
      perror("send");
      exit(EXIT_FAILURE);
    }
  }
}

int main(int argc, char ** argv) {
  std::vector<std::string> opts = {"port"};
  InputParser parser;
  std::unordered_map<std::string, int> parsedOpt = parser.parseOpt<int>(argc, argv, opts);

  Proxy p;
  int socketfd = p.proxyServerSetup(parsedOpt["port"]);
  p.serverBoot(socketfd);
}
