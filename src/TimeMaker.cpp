#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <ctime>
#include "TimeMaker.hpp"
#include <time.h>
double TimeMaker::getDateNowDiff(std::string time_str)
{
    time_t now;
    time(&now);
    double seconds;
    struct tm *now_struct = localtime(&now);
    struct tm time_struct;
    strptime(time_str.c_str(),"%a, %d %b %Y %H:%M:%S", &time_struct);
    seconds = difftime(mktime(&time_struct), mktime(&(*now_struct))); // end - begin
    return seconds;
}