#ifndef CACHECONTROLLER_HPP
#define CACHECONTROLLER_HPP
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <algorithm>
#include "LRUCache.hpp"

class CacheController
{
    LRUCache myCahe;
public:
    
    bool toRevalidate(std::string);
    bool checkCacheControl(std::string);
    bool checkPragma(std::string);
    bool checkVary(std::string);
    int getMaxage(std::string);
    bool checkExpires(std::string);
    CacheController(LRUCache);
    std::string controlParser();
};


#endif