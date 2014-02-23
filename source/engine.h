#ifndef _ENGINE_MODULE_
#define _ENGINE_MODULE_

#include "minimath.h"
#include "render.h"

#include <functional>
#include <unordered_map>

class engine
{
public:
    typedef std::function<void(float)> updHdl;
    typedef std::function<void(int)>   keyHdl;
    
    enum KEY 
    {
        LEFT  = 131,
        RIGHT = 132,
        DOWN  = 133,
        UP    = 134,
        SPACE = 57,
        ESC   = 61, 
    };    
    engine();
    ~engine();

    engine(const engine &) = delete;
    engine & operator = (const engine &) = delete;

    static material::ptr getMaterial(const std::string &key) {return materials[key];}

    void setKeyHandler(const keyHdl &hdl)       {onKey=hdl;}
    void setUpdateHandler(const updHdl &hdl) {onUpdate=hdl;}
    void mainLoop() {while(update()) {} }
private:
    void _init();
    void _update();
    bool update();
    void resize();
    
    updHdl onUpdate = updHdl();
    keyHdl    onKey = keyHdl();
    unsigned int width  = 1080/2;
    unsigned int height = 1920/2;
    float deltaTime = 0;
    static std::unordered_map<std::string, material::ptr> materials;
  
};






#endif