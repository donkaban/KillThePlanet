#ifndef _RENDER_MODULE_
#define _RENDER_MODULE_

#include "minimath.h"
#include "utils.h"
#include <stdexcept>

#include <vector>
#include <cstring>
#include <initializer_list>
#include <string>
#include <memory>
#include <unordered_map>

#if defined(PLATFORM_ANDROID)
    #include <GLES2/gl2.h>
#elif defined(PLATFORM_LINUX)
    #include <GL/gl.h>
    #include <GL/glext.h>
#endif   

typedef const std::string & strref;   

namespace shaders
{
    extern std::string vertex_base;
    extern std::string fragment_base;
}

struct vertex
{
    union
    {
        struct {float position[3],texcoord[2],normal[3];};
        float data[8];
    };
    inline const float * ptr() const {return position;}
    inline float       * ptr() {return position;}
    vertex(std::initializer_list<float> l) 
    {
        std::memcpy(data,l.begin(),sizeof(vertex));
    }   
};


class material : public std::enable_shared_from_this<material>
{
public: 
    typedef std::shared_ptr<material> ptr;
    typedef const ptr &               ref; 
  
    material();
    material(strref,strref,strref);
    virtual ~material();

    void    bind();   
    void    unbind();
    GLuint  getID() const;

private:
    GLuint      id;
    std::string v_shader;
    std::string f_shader;

};

class object : public std::enable_shared_from_this<object>
{
public: 
    typedef std::shared_ptr<object> ptr;
    typedef const ptr &       ref; 

    object(const std::vector<vertex> &, const std::vector<uint16_t> &);
    virtual ~object();
   
    ptr  get() ;
    void bind();     
    void unbind(); 
    virtual void render(); 
   
    void setMaterial(material::ref m);
    void setColor(const vec &c)    {color = c;}
    void setPosition(const vec &p) {position = p;}
    void setRotation(const vec &r) {rotation = r;}
    
private:
    std::vector<vertex>   vertexes;
    std::vector<uint16_t> indecies;        
    material::ptr         mat;
    GLuint                id[2];
    vec                   color; 
    vec                   position;
    vec                   rotation;

};  



#endif