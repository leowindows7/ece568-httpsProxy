#ifndef HTTPPARSER_HPP
#define HTTPPARSER_HPP

#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

class HttpParser {
 public:
  std::map<std::string, std::string> httpResMap(std::string header);
  std::string send200OK();
};
#endif
