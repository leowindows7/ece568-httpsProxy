#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <ctime>
#include "TimeMaker.hpp"
#include <time.h>
double TimeMaker::getTimeInSec(std::string time_str)
{
    
    double seconds;  
    struct tm time_struct;
    struct tm time_struct_ref;
    std::string ref_time = "Fri, 12 Feb 2022 15:20:01";
    strptime(time_str.c_str(),"%a, %d %b %Y %H:%M:%S", &time_struct);
    strptime(ref_time.c_str(),"%a, %d %b %Y %H:%M:%S", &time_struct_ref);
    seconds = difftime(mktime(&time_struct), mktime(&time_struct_ref));
    return seconds;
}