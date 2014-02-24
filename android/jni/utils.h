#ifndef _UTILS_MODULE_
#define _UTILS_MODULE_

#include <cstdio>
#include <chrono>
#include <thread>
#include <mutex>

#ifdef PLATFORM_ANDROID
    #include <android/log.h>
    #define  NFO(...)  __android_log_print(ANDROID_LOG_DEBUG,"[KtP]",__VA_ARGS__)
    #define  ERR(...)  __android_log_print(ANDROID_LOG_ERROR,"[KtP]",__VA_ARGS__)
#else
    #define  NFO(...)  printf("[nfo] "); printf(__VA_ARGS__); printf("\n"); 
    #define  ERR(...)  printf("[err] "); printf(__VA_ARGS__); printf("\n");
#endif 


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