/// @file   WhatTimeIsItNow.h
/// @brief  The module which gets the string of current Time.
/// @author Dohyun Yun

#ifndef _WHATTIMEISITNOW_H_
#define _WHATTIMEISITNOW_H_

#include <iostream>

#if defined(WIN32)
#   ifndef _WINSOCKAPI_
#   define _WINSOCKAPI_
#   endif
#   include <windows.h>
#else
#include <sys/time.h>
#endif

class WhatTimeIsItNow {
    
    public:
    static std::string main()
{
    char time_string[40];

#ifdef WIN32
    SYSTEMTIME currentTime;
    ::GetLocalTime(&currentTime);
    sprintf(time_string, "%4ld-%02ld-%02ld %02ld:%02ld:%02ld",
            currentTime.wYear, currentTime.wMonth, currentTime.wDay,
            currentTime.wHour, currentTime.wMinute, currentTime.wSecond);
#else
    struct timeval timeval;
    gettimeofday(&timeval, 0);
    struct tm* currentTime;
    currentTime = localtime( &timeval.tv_sec );

    sprintf(time_string, "%4d-%02d-%02d %02d:%02d:%02d",
            currentTime->tm_year + 1900, currentTime->tm_mon + 1, currentTime->tm_mday,
            currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec);
#endif
    std::cout << "CURRENT TIME : " << time_string << std::endl;
    return 0;
}
#endif //_WHATTIMEISITNOW_H_

