#ifndef _ENGINE_MODULE_
#define _ENGINE_MODULE_

#include "minimath.h"
#include "render.h"

#include <functional>
#include <list>

class engine
{
public:
    typedef std::function<void(float)>      updatehandler_t;
    typedef std::function<void(int)>        keyhandler_t;
    enum KEY 
    {
        LEFT  = 131,
        RIGHT = 132,
        DOWN  = 133,
        UP    = 134,
        SPACE = 57 
    };    
    engine();
    ~engine();

    engine(const engine &) = delete;
    engine & operator = (const engine &) = delete;

    bool update();
    void resize();

    inline void setKeyHandler(const keyhandler_t &hdl)            {onKey=hdl;}
    inline void setUpdateHandler(const updatehandler_t &hdl)      {onUpdate=hdl;}
   
    void addObject(object::ref obj)     {renderQueue.push_back(obj);}
    void removeObject(object::ref obj)  {}// todo!
private:

    void _init();
    void _update();

    std::list<object::ptr> renderQueue;
   
    updatehandler_t onUpdate        = updatehandler_t();
    keyhandler_t    onKey           = keyhandler_t();
    unsigned int width  = 1080/2;
    unsigned int height = 1920/2;
    float deltaTime = 0;
};






#endif