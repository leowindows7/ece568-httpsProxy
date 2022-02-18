#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <algorithm>
//#include <thread>
//#include <pthread.h>
#include "LRUCache.hpp"



std::map<std::string, std::string> LRUCache::getResponse(std::string url_string)
{
    std::vector<std::string>::iterator it = std::find(url_vector.begin(), url_vector.end(), url_string);
    int index = it - url_vector.begin();
    if (it == url_vector.end())
    {
        // remember to check if returned map size is 0 or not
        std::map<std::string, std::string> noSuchUrl;
        return noSuchUrl;
    }
    return res_Map_vector[index];
}

void LRUCache::getCapacity()
{
    std::cout << this->capacity << std::endl;
}

LRUCache::LRUCache(unsigned n)
{
    capacity = n;
}

void LRUCache::put(std::string url_string, std::map<std::string, std::string> response_Map)
{
    
    std::vector<std::string>::iterator it = std::find(url_vector.begin(), url_vector.end(), url_string);
    int index = it - url_vector.begin();
  
    if (it == url_vector.end())
    {
        if (url_vector.size() < capacity)
        {
            std::cout << "cache not full" << std::endl;
  
            url_vector.push_back(url_string);
            res_Map_vector.push_back(response_Map);
  
        }
        else
        {
            std::cout << "cache full" << std::endl;
            url_vector.erase(url_vector.begin());
            url_vector.push_back(url_string);
            res_Map_vector.erase(res_Map_vector.begin());
            res_Map_vector.push_back(response_Map);
            
        }
    }
    else // url already exist
    {
        std::cout << *it << " already exist" << std::endl;
       
        url_vector.erase(it);
        res_Map_vector.erase(res_Map_vector.begin() + index);
        url_vector.push_back(url_string);
        res_Map_vector.push_back(response_Map);
        
    }
   
}
void LRUCache::display()
{
    std::cout << "element in url vec" << std::endl;
    for (auto it = url_vector.begin(); it != url_vector.end(); it++)
    {
        std::cout << (*it) << " ";
    }
    std::cout << std::endl;
}
