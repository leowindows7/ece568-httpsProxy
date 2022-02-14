#include "proxy.hpp"

#include <cstring>
#include <iostream>
#include <map>
#include <string>
#include <thread>

#include "httpParser.hpp"
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
    std::map<std::string, std::string> headerMap = httpResMap(buf);
    std::string hostname = headerMap["Host"];
    int port = headerMap["Port"];
    dispatch_worker(hostname, port, client_connection_fd);
    // sendToHost(hostname, port, client_connection_fd);
  }
}

void sendToHost(std::string hostname, int port, int socketfd) {
  Client c;
  char * recvbuf = NULL;
  recvbuf = c.connectToHost(hostname, port);
  if (send(socketfd, recvbuf, strlen(recvbuf), 0) == -1) {
    perror("send");
    exit(EXIT_FAILURE);
  }
}

void Proxy::dispatch_worker(std::string hostname, int port, int socketfd) {
  std::thread(sendToHost, hostname, port, socketfd).detach();
}

int main(int argc, char ** argv) {
  std::vector<std::string> opts = {"port"};
  InputParser parser;
  std::unordered_map<std::string, int> parsedOpt = parser.parseOpt<int>(argc, argv, opts);

  Proxy p;
  int socketfd = p.proxyServerSetup(parsedOpt["port"]);
  p.serverBoot(socketfd);
}
