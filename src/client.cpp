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
                                  std::string http_request,
                                  int socket_fd) {
  Network::sendRequest(socket_fd, http_request.c_str(), http_request.size());

  std::string response = Network::recvRequest(socket_fd);
  HttpParser parser;
  std::map<std::string, std::string> responseMap = parser.httpResMap(response);

  std::string recvbuf;
  // handle Chunked data
  if (responseMap["transfer-encoding"].find("chunked") != std::string::npos) {
    recvbuf = handleChunkData(response, socket_fd);
    std::cout << recvbuf.size() << std::endl;
  }

  else {
    recvbuf = handleContentLength(response, socket_fd);
  }

  return recvbuf;
}

std::string handleContentLength(std::string response, int socket_fd) {
  // find the end of header
  std::string validResponse = response;
  while (validResponse.find("\r\n\r\n") == std::string::npos) {
    std::string tmp = Network::recvRequest(socket_fd);
    validResponse.append(tmp);
  }

  HttpParser parser;
  std::map<std::string, std::string> parsed = parser.httpResMap(validResponse);
  int contentLength = (parsed.find("content-length") != parsed.end())
                          ? std::stoi(parsed["content-length"])
                          : -1;
  if (contentLength == -1) {
    throw std::exception();
  }

  // start to parse data
  Network::assembleValidResponse(socket_fd, validResponse, contentLength);
  return validResponse;
}

std::string handleChunkData(std::string responseHeader, int socket_fd) {
  std::string chunkResponse;
  chunkResponse.append(responseHeader);

  char buffer[MAX_MSG_LENGTH] = {0};

  int num_bytes = 0;
  int sum = 0;
  while (1) {
    if ((num_bytes = recv(socket_fd, buffer + sum, sizeof(buffer) - num_bytes, 0)) ==
        -1) {
      perror("recv chunked");
      throw std::exception();
    }
    sum += num_bytes;

    std::cout << num_bytes << std::endl;

    std::string tmp(buffer);
    if (num_bytes == 5) {
      chunkResponse.append(buffer);
      break;
    }
  }

  std::cout << chunkResponse.size() << std::endl;
  return chunkResponse;
}

int Client::setUpSocket(std::string hostname, int port_num) {
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

  return socket_fd;
}
