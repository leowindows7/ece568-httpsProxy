#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <algorithm>
#include "LRUCache.hpp"
#include "CacheController.hpp"

bool CacheController::toRevalidate(std::string url_string)
{
    std::map<std::string, std::string> responseMap = myCahe.getResponse(url_string);
    if (responseMap.empty())
    {
        // no response in cache
        return true;
    }
    std::string cacheControl_str("");
    std::string pragma_str("");
    std::string vary_str("");
    if (responseMap.find("cache-control") != responseMap.end())
    {
        cacheControl_str = responseMap.find("cache-control")->second;
    }
    if (responseMap.find("pragma") != responseMap.end())
    {
        pragma_str = responseMap.find("pragma")->second;
    }
    if (responseMap.find("vary") != responseMap.end())
    {
        vary_str = responseMap.find("vary")->second;
    }
    if(responseMap.find("etag") != responseMap.end()){
        return true;
    }


    return checkCacheControl(cacheControl_str) || checkPragma(pragma_str) ||
           checkVary(vary_str);
}

bool checkCacheControl(std::string cacheControl_str)
{
    std::string targetHeader[]{"no-store", "no-cache", "must-revalidate"};
    std::vector<std::string> header_vec(targetHeader, targetHeader + sizeof(targetHeader) / sizeof(std::string));
    if (cacheControl_str.length() == 0)
    {
        return true;
    }
    for (auto it = header_vec.begin(); it != header_vec.end(); it++)
    {
        int pos = cacheControl_str.find(*it);
        if (pos != std::string::npos)
        {
            return true;
        }
    }
    return false;
}
bool checkPragma(std::string pragma_str)
{
    std::string targetHeader[]{"no-cache"};
    std::vector<std::string> header_vec(targetHeader, targetHeader + sizeof(targetHeader) / sizeof(std::string));
    if (pragma_str.length() == 0)
    {
        return true;
    }
    for (auto it = header_vec.begin(); it != header_vec.end(); it++)
    {
        int pos = pragma_str.find(*it);
        if (pos != std::string::npos)
        {
            return true;
        }
    }
    return false;
}
bool checkVary(std::string vary_str)
{
    if (vary_str.length() == 0)
    {
        return true;
    }
    return false;
}