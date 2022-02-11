#include "server.hpp"

#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "network.hpp"

#define RINGMASTER_COMMAND_LENGTH 2

std::unordered_map<std::string, int> parseOpt(int argc,
                                              char ** argv,
                                              std::vector<std::string> opts) {
  if (argc != RINGMASTER_COMMAND_LENGTH) {
    // TODO: refactor this to throw exception
    std::cerr << "Error: ringmaster <port_num>" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::unordered_map<std::string, int> parsedOpt;
  for (int i = 1; i < argc; i++) {
    std::stringstream ss;
    ss << argv[i];
    int tmp = 0;
    ss >> tmp;

    if (ss.fail()) {
      // TODO: throw exception
      fprintf(stderr, "Exception: invalid converion for parameter %s\n", argv[i]);
      exit(EXIT_FAILURE);
    }

    std::string option =
        opts[i - 1];  // argv corresponding value is 1 more that parsedOpt
    parsedOpt[option] = tmp;
  }

  return parsedOpt;
}

void Server::setupServer(const int port_num) {
  Network * connect = new Network();
  std::pair<int, struct addrinfo *> socketInfo =
      connect->connectSetup<int, struct addrinfo *>(port_num);

  int socket_fd = socketInfo.first;
  struct addrinfo * serviceinfo = socketInfo.second;

  // std::cout << socket_fd << std::endl;

  if (bind(socket_fd, serviceinfo->ai_addr, serviceinfo->ai_addrlen) == -1) {
    // TODO: throw exception
    perror("bind");
    exit(EXIT_FAILURE);
  }

  int yes = 1;
  if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
    // TODO: throw exception
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  freeaddrinfo(serviceinfo);

  if (listen(socket_fd, 10) == -1) {
    // TODO: refactor this
    perror("listen");
    exit(EXIT_FAILURE);
  }

  while (1) {
    sleep(1);
    struct sockaddr_storage their_addr;
    socklen_t addr_size = sizeof(their_addr);
    int new_fd = accept(socket_fd, (struct sockaddr *)&their_addr, &addr_size);
    std::cout << new_fd << std::endl;

    char buf[4096] = {0};
    int num_bytes = 0;
    if ((num_bytes = recv(new_fd, buf, sizeof(buf), 0)) == -1) {
      perror("recv");
      exit(EXIT_FAILURE);
    }

    buf[num_bytes] = '\0';
    std::cout << buf << std::endl;

    if (close(socket_fd) == -1) {
      // TODO: throw exception
      perror("close");
      exit(EXIT_FAILURE);
    }
  }
}

int main(int argc, char ** argv) {
  std::vector<std::string> opts = {"port"};
  std::unordered_map<std::string, int> parsedOpt = parseOpt(argc, argv, opts);

  Server server;
  server.setupServer(parsedOpt["port"]);
}
