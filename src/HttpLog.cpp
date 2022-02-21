#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include "HttpLog.hpp"
pthread_mutex_t lock;

void HttpLog::openLogFile()
{
    if (!logFile.is_open())
    {
        logFile.open("log.txt", std::ofstream::app);
    }
}

void HttpLog::closeLogFile()
{
    logFile.close();
}

void HttpLog::writeLog(std::string toWrite)
{
    if (logFile.is_open())
    {
        pthread_mutex_lock(&lock);
        logFile << toWrite << std::endl;
        pthread_mutex_unlock(&lock);
    }
}