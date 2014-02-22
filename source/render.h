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
    extern std::string fragment_base;
}

class material : public std::enable_shared_from_this<material>
{
public: 
    typedef std::shared_ptr<material> ptr;
    typedef const ptr &               ref; 
  
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
    typedef float vertex[8];

public: 
    typedef std::shared_ptr<object> ptr;
    typedef const ptr &             ref; 

    object(const std::vector<float> &, const std::vector<uint16_t> &);
    virtual ~object();
   
    ptr  get() ;
    void bind();     
    void unbind(); 
    virtual void render(); 
   
    void setMaterial(material::ref m);
    
    void setColor(const vec &);
    vec  getColor() const;
    void rotate(const vec &);

    static ptr plane(const vec &);
    static ptr cube(const vec &);
    static ptr batman(const vec &);

    static ptr sphere(float, int );
   
private:
    GLuint                id[2];
    std::vector<float>    vertexes;
    std::vector<uint16_t> indecies;        
  
    material::ptr         mat;
    vec                   color = vec({1,1,1,1}); 
    mat4                  transform; 

};  



#endif