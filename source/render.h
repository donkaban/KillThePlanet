#ifndef _RENDER_MODULE_
#define _RENDER_MODULE_

#include "minimath.h"
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

////////////////////////////////////////////////////////// Vertex     ////////////////////////

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

////////////////////////////////////////////////////////// Material ////////////////////////

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
    
    static ptr get(strref);

private:
    GLuint      id;
    std::string v_shader;
    std::string f_shader;

    static std::unordered_map<std::string,ptr> cache;
};

////////////////////////////////////////////////////////// Renderable Object ////////////////////////

class object : public std::enable_shared_from_this<object>
{
public: 
    typedef std::shared_ptr<object> ptr;
    typedef const ptr &       ref; 

    object(const std::vector<vertex> &v, const std::vector<uint16_t> &i);
    virtual ~object();
   
    void bind();     
    void unbind(); 
    void setMaterial(material::ref m);
    ptr  get();

    virtual void render(); 
    
private:
    std::vector<vertex>   vertexes;
    std::vector<uint16_t> indecies;        
    material::ptr         mat;
    GLuint                id[2];

};  



#endif