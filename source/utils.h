#ifndef _UTILS_MODULE_
#define _UTILS_MODULE_
#include <cstdio>

#ifdef PLATFORM_ANDROID
    #include <android/log.h>
    #define  _NFO(...)  __android_log_print(ANDROID_LOG_DEBUG,   "[KtP]",__VA_ARGS__)
    #define  _ERR(...)  __android_log_print(ANDROID_LOG_ERROR,   "[KtP]",__VA_ARGS__)
#else
    #define  _NFO(...)  printf("[nfo] "); printf(__VA_ARGS__); printf("\n"); 
    #define  _ERR(...)  printf("[err] "); printf(__VA_ARGS__); printf("\n");
#endif 

class logger
{
public:
	template<typename ... T>
    static void info(const char* fmt, T && ... args) {_NFO(fmt, args ...);}

	template<typename ... T>
    static void error(const char* fmt, T && ... args){_ERR(fmt, args ...);}
};


#endif