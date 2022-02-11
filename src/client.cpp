#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <vector>
#include <unistd.h>
#include <fstream>

using namespace std;

int main(int argc, char *argv[])
{
    int status;
    int socket_fd;
    struct addrinfo host_info;
    struct addrinfo *host_info_list;
    const char *hostname = argv[1];
    const char *port = "80";

    if (argc < 2)
    {
        cout << "Syntax: client <hostname>\n"
             << endl;
        return 1;
    }

    memset(&host_info, 0, sizeof(host_info));
    host_info.ai_family = AF_UNSPEC;
    host_info.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(hostname, port, &host_info, &host_info_list);
    if (status != 0)
    {
        cerr << "Error: cannot get address info for host" << endl;
        cerr << "  (" << hostname << "," << port << ")" << endl;
        return -1;
    } //if

    socket_fd = socket(host_info_list->ai_family,
                       host_info_list->ai_socktype,
                       host_info_list->ai_protocol);
    if (socket_fd == -1)
    {
        cerr << "Error: cannot create socket" << endl;
        cerr << "  (" << hostname << "," << port << ")" << endl;
        return -1;
    } //if

    cout << "Connecting to " << hostname << " on port " << port << "..." << endl;

    status = connect(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if (status == -1)
    {
        cerr << "Error: cannot connect to socket" << endl;
        cerr << "  (" << hostname << "," << port << ")" << endl;
        return -1;
    } //if
    std::string line;
    std::vector<std::string> lines;
    //std::istream & is;
    //

    //const char *message = "hi there!";

    //std::istream ifs;
    string url = "www.google.com";
    string get_http = "GET / HTTP/1.1\nHost: " + 
    url + 
    "\nConnection: close\n\n";
    char buffer[10000];
    int nDataLength;
    string website_HTML;
    send(socket_fd,get_http.c_str(), strlen(get_http.c_str()),0 );
    recv(socket_fd, buffer, sizeof(buffer), 0);
    while ((nDataLength = recv(socket_fd,buffer,10000,0)) > 0){        
        int i = 0;
        while (buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r'){
            website_HTML+=buffer[i];
            i += 1;
        }               
    }
    
    cout <<  website_HTML << endl;
    freeaddrinfo(host_info_list);
    close(socket_fd);

    return 0;
}
