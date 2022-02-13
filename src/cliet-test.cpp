#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "client.hpp"
#include "inputParser.hpp"
int main(int argc, char ** argv) {
  std::vector<std::string> opts = {"hostname", "port"};
  InputParser parser;
  std::unordered_map<std::string, std::string> parsedOpt =
      parser.parseOpt<std::string>(argc, argv, opts);
  for (auto iter = parsedOpt.begin(); iter != parsedOpt.end(); iter++) {
    std::cout << iter->first << ": " << iter->second << std::endl;
  }

  Client client;
  std::string hostname = parsedOpt["hostname"];
  int port = std::stoi(parsedOpt["port"]);
  char * recv = client.connectToHost(hostname, port);
  std::cout << recv << std::endl;
  delete[] recv;
}
