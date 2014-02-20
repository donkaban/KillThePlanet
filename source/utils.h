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

namespace aux
{
	extern std::mutex globalLocker;
};

class logger
{
public:
	template<typename ... T>
    static void info(const char* fmt, T && ... args) {_NFO(fmt, args ...);}

	template<typename ... T>
    static void error(const char* fmt, T && ... args){_ERR(fmt, args ...);}
};

class looped_task
{
	typedef std::chrono::time_point<std::chrono::system_clock> time_p;
	typedef std::function<void()> callback_t;
public:
	looped_task(float delta, const callback_t &task)
	{
		logger::info("start looped task");
		thr = std::thread([delta,&task,this]()
		{
			time_p start = std::chrono::system_clock::now();
			time_p current; 
			while(!done)
			{
				current = std::chrono::system_clock::now();
    			std::chrono::duration<float> dt(current - start);
    			if(dt.count() >= delta)
     			{	
				 	std::lock_guard<std::mutex> lock(aux::globalLocker);
				 	task();
				 	start = current;
				}
			}	
			logger::info("kill looped task");
		});
	
	}

	~looped_task()
	{
		done = true;
		if(thr.joinable()) thr.join();
	}

private:
	std::thread thr;
	bool done = false;	
};



#endif