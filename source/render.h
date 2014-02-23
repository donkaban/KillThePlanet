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

class material 
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

class object 
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
    
    void color(vec::ref);
    void rotate(vec::ref);
    void translate(vec::ref);
    void scale(vec::ref);
    void position(vec::ref); 
    void transform(mat::ref); 
   
    vec      position()  const;
    mat::ref transform() const;
  
    static ptr cube(vec::ref);
     
private:
    GLuint                id[2];
    std::vector<float>    vertexes;
    std::vector<uint16_t> indecies;        
  
    material::ptr _material;
    vec           _color = vec({1,1,1,1}); 
    mat           _transform; 
};  



#endif