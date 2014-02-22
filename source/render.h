#ifndef _RENDER_MODULE_
#define _RENDER_MODULE_

#include "minimath.h"
#include "utils.h"

#include <vector>
#include <cstring>
#include <initializer_list>
#include <string>
#include <memory>
#
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
    extern std::string ship;
    extern std::string bullet;
}

class material : public std::enable_shared_from_this<material>
{
public: 
    typedef std::shared_ptr<material> ptr;
    typedef const ptr &               ref; 
  
    int pos,uv,time,mv,col; 
  
    material();
    material(strref,strref);
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
    typedef float vertex[5];

public: 
    typedef std::shared_ptr<object> ptr;
    typedef const ptr &             ref; 

    object(const std::vector<float> &, const std::vector<uint16_t> &);
    virtual ~object();
   
    void bind();     
    void unbind(); 
    void render(); 
   
    void setMaterial(material::ref m);
    
    void color(const vec &);
    void rotate(const vec &);
    void translate(const vec &);
    void scale(const vec &);
    void position(const vec &); 
    vec  position() const ;
    void transform(const mat4 &); 
    const mat4 & transform() const ;
  
    static ptr cube(const vec &);
     
private:
    GLuint                id[2];
    std::vector<float>    vertexes;
    std::vector<uint16_t> indecies;        
  
    material::ptr         mat;
    vec                   col = vec({1,1,1,1}); 
    mat4                  _transform; 
};  



#endif