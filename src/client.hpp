#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <memory>
#include <string>

#include "network.hpp"

class Client {
  std::unique_ptr<Network> cconnect;

 public:
  Client() : cconnect(new Network) {}
  char * connectToHost(std::string hostname, int port);
};
#endif