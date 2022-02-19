#ifndef CACHECONTROLLER_HPP
#define CACHECONTROLLER_HPP
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "LRUCache.hpp"

<<<<<<< HEAD
class CacheController
{
    LRUCache myCahe;
public:
    
    bool toRevalidate(std::string, std::map<std::string, std::string>);
    bool checkCacheControl(std::string);
    bool checkPragma(std::string);
    bool checkVary(std::string);
    int getMaxage(std::string);
    bool checkExpires(std::string);
    void putInCache(std::string, std::map<std::string, std::string>);
    CacheController(LRUCache);
    std::string controlParser();
};
=======
class CacheController {
  LRUCache myCahe;
  bool checkCacheControl(std::string);
  bool checkPragma(std::string);
  bool checkVary(std::string);
  int getMaxage(std::string);
  bool checkExpires(std::string);
  CacheController(LRUCache);
  std::string controlParser();
>>>>>>> save

 public:
  CacheController() {}
  bool toRevalidate(std::string, std::map<std::string, std::string>);
};

#endif
