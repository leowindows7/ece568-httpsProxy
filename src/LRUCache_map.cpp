#include <bits/stdc++.h>
// using namespace std;

class LRUCache
{
    // store keys of cache
    std::list<std::string> dq;
    // store references of key in cache
    std::unordered_map<std::string, std::list<std::string>::iterator> u_map;
    int max_size; // maximum capacity of cache

public:
    LRUCache(int);
    void put(std::string);
    void getResponse(std::string);
    void display();
};


// LRUCache::getResponse(std::string request){
//    std::cout <<*u_map[request] <<std::endl;
//     //return *u_map[request];
// }

// Declare the size
LRUCache::LRUCache(int n)
{
    max_size = n;
}

// Refers key x with in the LRU cache
void LRUCache::put(std::string x)
{
    //  in cache
    if (u_map.find(x) != u_map.end())
    {
        dq.erase(u_map[x]);
    } // not in cache
    else
    {
        // cache is full
        if (dq.size() == max_size)
        {
            // delete least recently used element
            std::string last = dq.back();
            // Pops the last element
            dq.pop_back();
            // Erase the last
            u_map.erase(last);
        }
    }
    // update reference
    dq.push_front(x);
    u_map[x] = dq.begin();
}

// Function to display contents of cache
void LRUCache::display()
{
    // Iterate in the deque and print
    // all the elements in it
    for (auto it = dq.begin(); it != dq.end(); it++)
    {
        std::cout << (*it) << " ";
    }
    std::cout << std::endl;
}

// Driver Code
int main()
{
    LRUCache ca(4);
    ca.put("1");
    ca.put("2");
    ca.put("3");
    ca.put("1");
    ca.put("4");
    ca.put("5");
    ca.display();
    //ca.getResponse("5");
    return 0;
}
// This code is contributed by Satish Srinivas
