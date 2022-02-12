#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <string>

class Client {
 public:
  void connectToHost(std::string hostname, int port);
};
#endif
