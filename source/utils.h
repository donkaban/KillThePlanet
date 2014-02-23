#ifndef _UTILS_MODULE_
#define _UTILS_MODULE_

#include <cstdio>
#include <chrono>
#include <thread>
#include <mutex>

#ifdef PLATFORM_ANDROID
    #include <android/log.h>
    #define  _NFO(...)  __android_log_print(ANDROID_LOG_DEBUG,   "[KtP]",__VA_ARGS__)
    #define  _ERR(...)  __android_log_print(ANDROID_LOG_ERROR,   "[KtP]",__VA_ARGS__)
#else
    #define  _NFO(...)  printf("[nfo] "); printf(__VA_ARGS__); printf("\n"); 
    #define  _ERR(...)  printf("[err] "); printf(__VA_ARGS__); printf("\n");
#endif 


struct logger
{
	template<typename ... T>
    static void info(const char* fmt, T && ... args) {_NFO(fmt, args ...);}

	template<typename ... T>
    static void error(const char* fmt, T && ... args){_ERR(fmt, args ...);}
};

struct timer
{
private:
	static std::chrono::time_point<std::chrono::system_clock> start_time;
public:	
 	static float get() 
    {
        std::chrono::duration<float> dt(std::chrono::system_clock::now() - start_time);
        return dt.count();
    }

};

template <typename T>
static void updater(T & queue, float dt)
{
    for (auto i = queue.begin(); i != queue.end();)
    {
        if((*i)->update(dt)) 
        {   
            delete *i;
            i = queue.erase(i);
        }
        else i++;
    }   
}        


#endif