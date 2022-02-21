#ifndef HTTPLOG_HPP
#define HTTPLOG_HPP
#include <iostream>
#include <string>
#include <fstream>
class HttpLog
{
private:
    std::ofstream logFile;
public:
    HttpLog () {}
    // HttpLog(const HttpLog &rhs){
    //     &this->logFile = &rhs.logFile;
    // }
    // HttpLog& operator= (const HttpLog &rhs){
    //     if (this != &rhs){
    //         &this->logFile = &rhs.logFile;
    //     }
    // }
    void openLogFile();
    void writeLog(std::string);
    void closeLogFile();
};
#endif