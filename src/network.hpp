#ifndef NETWORK_H
#define NETWORK_H

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <cstring>
#include <utility>

class Network {
 public:
  template<typename T, typename U>
  std::pair<T, U> connectSetup(const char * hostName, int port_num) {
    struct addrinfo hints;
    struct addrinfo * serviceinfo;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int status = 0;
    if ((status = getaddrinfo(
             hostName, std::to_string(port_num).c_str(), &hints, &serviceinfo)) != 0) {
      // TODO: throw exception
      fprintf(stderr, "Error: getaddrinfo error\n");
      exit(EXIT_FAILURE);
    }

    // TODO: refactor to loop through linked list
    int socket_fd = -1;
    if ((socket_fd = socket(serviceinfo->ai_family,
                            serviceinfo->ai_socktype,
                            serviceinfo->ai_protocol)) == -1) {
      // TODO: throw exception
      perror("socket error");
      exit(EXIT_FAILURE);
    }

    std::pair<T, U> connectInfo(socket_fd, serviceinfo);
    return connectInfo;
  }
};

#endif