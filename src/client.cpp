#include "client.hpp"

#include <cstdlib>
#include <exception>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "httpParser.hpp"
#include "network.hpp"

/** Connect to specified host
 * @param hostname: hostname to connect
 * @param port_num: port number
 * @param http_request
 * @return char * need to free by programmer
 **/
std::string Client::connectToHost(std::string hostname,
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

  Network::sendRequest(socket_fd, http_request.c_str(), http_request.size());

  int numbytes = 0;
  std::string s = Network::recvRequest(socket_fd);
  std::cout << "numbytes " << numbytes << std::endl << s.size() << std::endl;

  return s;
}

void assembleHeaderBody(char * recvbuffer, int numbytes, int socket_fd) {
  HttpParser parser;
  std::map<std::string, std::string> hashMap = parser.httpResMap(recvbuffer);
  int bodyExpectedLength = std::stoi(hashMap["Content-Length"]);
  // means https response
  std::string s(recvbuffer);
  size_t pos = 0;
  std::string body;
  if ((pos = s.find("\r\n\r\n")) != std::string::npos) {
    // get current body
    char * bodyStart = recvbuffer + pos + strlen("\r\n\r\n");
    int bodyBytes = numbytes;
    body += bodyStart;
    while (bodyBytes < bodyExpectedLength) {
      if ((numbytes = recv(socket_fd, recvbuffer, MAX_MSG_LENGTH, 0)) == -1) {
        perror("recv");
        throw std::exception();
      }
      recvbuffer[numbytes] = '\0';
      bodyBytes += numbytes;
      body += recvbuffer;
    }
  }
}
