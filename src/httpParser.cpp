#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <vector>
#include <unistd.h>
#include <fstream>
#include <map>
using namespace std;

map<string, string> httpResMap(string response)
{
    map<string, string> responseMap;
    size_t pos = 0;
    string delimiter = "\n";
    string token;
    response.find_first_not_of(delimiter);
    pos = response.find(delimiter);
    token = response.substr(0, pos);
    responseMap.insert({"Status", token});
    response.erase(0, pos + delimiter.length());
    string header = "";
    while ((pos = response.find(delimiter)) != 1)
    {
        token = response.substr(0, pos);
        header += token + "\n";
        response.erase(0, pos + delimiter.length());
    }
    response.erase(0, pos + delimiter.length());
    responseMap.insert({"Header", header});
    responseMap.insert({"Body", response});
    return responseMap;
}

// int main(int argc, char *argv[])
int main()
{
    string url = "www.google.com";
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
        cerr << "Error: cannot get address info for host" << endl;
        cerr << "  (" << hostname << "," << port << ")" << endl;
        return -1;
    } // if

    socket_fd = socket(host_info_list->ai_family,
                       host_info_list->ai_socktype,
                       host_info_list->ai_protocol);
    if (socket_fd == -1)
    {
        cerr << "Error: cannot create socket" << endl;
        cerr << "  (" << hostname << "," << port << ")" << endl;
        return -1;
    } // if

    cout << "Connecting to " << hostname << " on port " << port << "..." << endl;

    status = connect(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if (status == -1)
    {
        cerr << "Error: cannot connect to socket" << endl;
        cerr << "  (" << hostname << "," << port << ")" << endl;
        return -1;
    } // if
    string get_http = "GET / HTTP/1.1\nHost: " +
                      url + "\nConnection: keep-alive\n\n";
    char buffer[65536];
    int nDataLength;
    send(socket_fd, get_http.c_str(), strlen(get_http.c_str()), 0);
    nDataLength = recv(socket_fd, buffer, sizeof(buffer), 0);
    if (nDataLength == -1)
    {
        cerr << "receive fail" << endl;
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
    string website_response(buffer);
    map<string, string> responseMap = httpResMap(website_response);
    cout << responseMap["Status"] <<endl;
    cout << responseMap["Header"] <<endl;
    cout << responseMap["Body"] <<endl;
    freeaddrinfo(host_info_list);
    close(socket_fd);

    return 0;
}
