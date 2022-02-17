#include "proxy.hpp"

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

void handleGetRequest(std::string hostname,
                      int port,
                      int socketfd,
                      std::string http_request) {
  Client c;
  std::string recvbuf;
  recvbuf = c.connectToHost(hostname, port, http_request);

  if (send(socketfd, recvbuf.c_str(), recvbuf.size(), 0) == -1) {
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
    std::cout << method << std::endl;

    // handle action
    if (method.find("GET") != std::string::npos) {
      handleGetRequest(hostname, port, client_connection_fd, http_request);
    }

    else if (method.find("CONNECT") != std::string::npos) {
      // handleConnectRequest();
    }
  }
}

void Proxy::dispatch_worker(int socketfd) {
  // std::cout << buf << std::endl;
  std::thread(handleNewTab, socketfd).detach();
}
// TODO: function unfinished
/*
void handlConnectRequest() {
  HttpParser httpParser;
  std::string recvbuf = httpParser.send200OK();
  if (send(socketfd, recvbuf.c_str(), strlen(recvbuf.c_str()), 0) == -1) {
    perror("send");
    exit(EXIT_FAILURE);
  }

  std::string recvBuf = processRequest(socketfd);
  std::cout << "client mesg " << recvBuf << std::endl;
}
*/

int main(int argc, char ** argv) {
  std::vector<std::string> opts = {"port"};
  InputParser parser;
  std::unordered_map<std::string, int> parsedOpt = parser.parseOpt<int>(argc, argv, opts);

  Proxy p;
  int socketfd = p.proxyServerSetup(parsedOpt["port"]);
  p.serverBoot(socketfd);
}
