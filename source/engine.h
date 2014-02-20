#ifndef _ENGINE_MODULE_
#define _ENGINE_MODULE_

#include <functional>
#include <list>
#include <chrono>


#include "minimath.h"
#include "render.h"

class engine
{
public:
    typedef std::function<void()>           updatehandler_t;
    typedef std::function<void(int, int)>   mousehandler_t;
    typedef std::function<void(int)>        keyhandler_t;
    
    engine();
    ~engine();

    engine(const engine &) = delete;
    engine & operator = (const engine &) = delete;

    bool update();
    void resize();

    inline void setKeyHandler(const keyhandler_t &hdl)            {onKey=hdl;}
    inline void setMousePressHandler(const mousehandler_t &hdl)   {onMousePress=hdl;}
    inline void setMouseReleaseHandler(const mousehandler_t &hdl) {onMouseRelease=hdl;}
    inline void setMouseMoveHandler(const mousehandler_t &hdl)    {onMouseMove=hdl;}
    inline void setUpdateHandler(const updatehandler_t &hdl)      {onUpdate=hdl;}

    void  setBackColor(const vec4 &);
    float getTime() 
    {
        std::chrono::duration<float> dt(std::chrono::system_clock::now() - _start_time);
        return dt.count();
    }

    void addObject(object::ref obj)     {renderQueue.push_back(obj);}
    void removeObject(object::ref obj)  {}// todo!
private:

    std::list<object::ptr> renderQueue;
   
    updatehandler_t onUpdate        = updatehandler_t();
    mousehandler_t  onMousePress    = mousehandler_t();
    mousehandler_t  onMouseRelease  = mousehandler_t();
    mousehandler_t  onMouseMove     = mousehandler_t();
    keyhandler_t    onKey           = keyhandler_t();

    unsigned int _width  = 1080/2;
    unsigned int _height = 1920/2;
    std::chrono::time_point<std::chrono::system_clock> _start_time = std::chrono::system_clock::now();
   
};






#endif