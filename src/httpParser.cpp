#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <vector>
#include <unistd.h>
#include <fstream>
#include <map>

std::map<std::string, std::string> httpResMap(std::string response)
{
    std::map<std::string, std::string> responseMap;
    size_t pos = 0;
    std::string delimiter = "\n";
    std::string token;
    std::string firstLine;
    response.find_first_not_of(delimiter);
    pos = response.find(delimiter);
    // find first line to check if it's response or request
    firstLine = response.substr(0, pos);
    // response HTTP/ will be at the begining of the first line
    int firstLine_pos = firstLine.find("HTTP/");
    if (firstLine_pos == 0)
    {
        responseMap.insert({"Type", "Response"});
        firstLine_pos = firstLine.find(" ");
        responseMap.insert({"StatusCode", firstLine.substr(firstLine_pos + 1, firstLine.length())});
    }
    else if (firstLine_pos > 0)
    {
        responseMap.insert({"Type", "Request"});
        firstLine_pos = firstLine.find(" ");
        responseMap.insert({"Method", firstLine.substr(0, firstLine_pos + 1)});
    }

    // get header
    response.erase(0, pos + delimiter.length());
    std::string header = "";
    while ((pos = response.find(delimiter)) != 1)
    {
        token = response.substr(0, pos);
        header += token + "\n";
        response.erase(0, pos + delimiter.length());
    }
    response.erase(0, pos + delimiter.length());
    // body
    responseMap.insert({"Body", response});
    // parse header, each attribute before : will be used as key
    std::string headerDeli = ":";
    std::string tokenKey;
    std::string tokenValue;
    while ((pos = header.find(headerDeli)) != std::string::npos)
    {
        tokenKey = header.substr(0, pos);
        // cout << tokenKey << endl;
        header.erase(0, pos + headerDeli.length());
        pos = header.find(delimiter);
        tokenValue = header.substr(1, pos - 1);
        // cout << tokenValue << endl;
        header.erase(0, pos + delimiter.length());
        responseMap.insert({tokenKey, tokenValue});
    }
    if(responseMap["Type"] == "Request"){
        if((pos = responseMap["Host"].find(":")) > 0){
            std::string tmp = responseMap["Host"];
            responseMap["Host"] = responseMap["Host"].substr(0, pos);
            responseMap.insert({"Port", tmp.substr(pos + 1, responseMap["Host"].length())});
        } else {
            responseMap.insert({"Port", "-1"});
        }
    }
    return responseMap;
}

// int main(int argc, char *argv[])
int main()
{
    std::string url = "www.google.com";
    int status;
    int socket_fd;
    struct addrinfo host_info;
    struct addrinfo *host_info_list;
    const char *hostname = url.c_str();
    const char *port = "80";

    // if (argc < 2)
    // {
    //     cout << "Syntax: client <hostname>\n"
    //          << endl;
    //     return 1;
    // }

    memset(&host_info, 0, sizeof(host_info));
    host_info.ai_family = AF_UNSPEC;
    host_info.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(hostname, port, &host_info, &host_info_list);
    if (status != 0)
    {
        std::cerr << "Error: cannot get address info for host" << std::endl;
        std::cerr << "  (" << hostname << "," << port << ")" << std::endl;
        return -1;
    } // if

    socket_fd = socket(host_info_list->ai_family,
                       host_info_list->ai_socktype,
                       host_info_list->ai_protocol);
    if (socket_fd == -1)
    {
        std::cerr << "Error: cannot create socket" << std::endl;
        std::cerr << "  (" << hostname << "," << port << ")" << std::endl;
        return -1;
    } // if

    std::cout << "Connecting to " << hostname << " on port " << port << "..." << std::endl;

    status = connect(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if (status == -1)
    {
        std::cerr << "Error: cannot connect to socket" << std::endl;
        std::cerr << "  (" << hostname << "," << port << ")" << std::endl;
        return -1;
    } // if
    std::string get_http = "GET / HTTP/1.1\nHost: " +
                           url + "\nConnection: keep-alive\n\n";
    char buffer[65536];
    int nDataLength;
    send(socket_fd, get_http.c_str(), strlen(get_http.c_str()), 0);
    nDataLength = recv(socket_fd, buffer, sizeof(buffer), 0);
    if (nDataLength == -1)
    {
        std::cerr << "receive fail" << std::endl;
    }
    // while ((nDataLength = recv(socket_fd, buffer, 512, 0)) > 0)
    //{
    //     int i = 0;
    //     while (buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r')
    //     {
    //         website_HTML += buffer[i];
    //         i += 1;
    //     }
    //}
    // buffer[511] = '\0';
    std::string website_response(buffer);
    std::map<std::string, std::string> responseMap = httpResMap(website_response);
    for (auto it = responseMap.begin(); it != responseMap.end(); ++it)
    {
        std::cout << it->first << ":" << it->second << std::endl;
    }
    freeaddrinfo(host_info_list);
    close(socket_fd);

    return 0;
}
