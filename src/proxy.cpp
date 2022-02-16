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
  HttpParser httpParser;
  while (1) {
    int client_connection_fd = acceptRequest(socketfd);
    std::string buf = processRequest(client_connection_fd);
    std::cout << "In Proxy:" << std::endl << buf << std::endl;  // TODO: remove this
    std::map<std::string, std::string> headerMap = httpParser.httpResMap(buf);
    std::string hostname = headerMap["Host"];
    int port = (std::stoi(headerMap["Port"]) == -1) ? 80 : std::stoi(headerMap["Port"]);
    std::string method = headerMap["Method"];
    dispatch_worker(hostname, port, client_connection_fd, buf);
    // sendToHost(hostname, port, client_connection_fd);
  }
}

void sendToHost(std::string hostname, int port, int socketfd, std::string http_request) {
  Client c;
  char * recvbuf = nullptr;
  recvbuf = c.connectToHost(hostname, port, http_request);
  std::cout << recvbuf << std::endl;

  /*
  HttpParser httpParser;
  std::string recvbuf = httpParser.send200OK();
  if (send(socketfd, recvbuf.c_str(), strlen(recvbuf.c_str()), 0) == -1) {
    perror("send");
    exit(EXIT_FAILURE);
  }
  */

  // std::string recvBuf = processRequest(socketfd);
  // std::cout << "client mesg " << recvBuf << std::endl;
}

void Proxy::dispatch_worker(std::string hostname,
                            int port,
                            int socketfd,
                            std::string http_request) {
  // std::cout << buf << std::endl;
  std::thread(sendToHost, hostname, port, socketfd, http_request).detach();
}

int main(int argc, char ** argv) {
  std::vector<std::string> opts = {"port"};
  InputParser parser;
  std::unordered_map<std::string, int> parsedOpt = parser.parseOpt<int>(argc, argv, opts);

  Proxy p;
  int socketfd = p.proxyServerSetup(parsedOpt["port"]);
  p.serverBoot(socketfd);
}
