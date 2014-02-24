#include "render.h"
#include "utils.h"

#include <memory>
#include <unordered_map> 
#include <cstdlib>
#include <stdexcept>

static void check(strref where = "somethere over the rainbow...")
{
    if(GL_NO_ERROR != glGetError())
        throw std::runtime_error("OpenGL error in " + where);
}
static void check(int id)
{
    int compiled = 0;
    glGetShaderiv (static_cast<GLuint>(id), GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
        GLint       lenght;
        std::string message;
        glGetShaderiv (static_cast<GLuint>(id), GL_INFO_LOG_LENGTH, &lenght);
        if (lenght > 1)
        {
            message.resize(static_cast<size_t>(lenght));
            glGetShaderInfoLog(id, lenght, NULL, &message[0]);
            glDeleteShader (id);
            ERR("can't compile shader: %s",message.c_str());
        }
    }
}

material::material() {}
material::material(strref vsh, strref fsh) : 
    v_shader(vsh),
    f_shader(fsh)
{
    bind();
}
material::~material()
{}

void material::bind()   
{
    #ifdef PLATFORM_ANDROID
        v_shader = "precision highp float;\n" + v_shader;
        f_shader = "precision highp float;\n" + f_shader;
    #endif

    id = glCreateProgram(); 

    auto vsh = glCreateShader(GL_VERTEX_SHADER);    
    auto fsh = glCreateShader(GL_FRAGMENT_SHADER);  
    if(vsh == 0 || fsh == 0)
        throw std::runtime_error("can't create shader, perhaps GL context not created");
    
    auto v_src = v_shader.c_str();
    auto f_src = f_shader.c_str();
    
    glShaderSource(vsh, 1, &v_src, NULL); 
    glShaderSource(fsh, 1, &f_src, NULL); 
    glCompileShader(vsh); check(vsh);
    glCompileShader(fsh); check(fsh);
  
    glAttachShader(id, vsh);
    glAttachShader(id, fsh);
    glLinkProgram(id); 
    pos  = glGetAttribLocation(id,"pos");
    uv   = glGetAttribLocation(id,"uv");
    col  = glGetUniformLocation(id,"color");
    mv   = glGetUniformLocation(id,"mView");
    time = glGetUniformLocation(id,"time");
    check("material::bind");
}
void material::unbind() {}
   
GLuint material::getID() const {return id;}


object::object(const std::vector<float> &v, const std::vector<uint16_t> &ndx, material::ref m) :
        _material(m),
        vertexes(v),
        indecies(ndx) 
{
    bind();
}
object::~object() {};
   
void object::bind()     
{
    glGenBuffers(2, id);
    glBindBuffer(GL_ARRAY_BUFFER, id[0]);
    glBufferData(GL_ARRAY_BUFFER, vertexes.size() * sizeof(float) , &vertexes[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,id[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indecies.size() * sizeof(uint16_t), &indecies[0], GL_STATIC_DRAW);
    check("object::bind");
}
void object::unbind() 
{
    glDeleteBuffers(2,id);
    check("object::unbind");
} 

void object::material(material::ref m) {_material = m;}

void object::render() 
{
    if(!_material) return;// {ERR("try render without material. its stupid."); return;}
    glUseProgram(_material->getID());

    if(_material->col  !=-1) glUniform4f(_material->col, _color.r, _color.g, _color.b, 1);
    if(_material->mv   !=-1) glUniformMatrix4fv(_material->mv, 1,GL_FALSE,_transform.data);
    if(_material->time !=-1) glUniform1f(_material->time, timer::get());

    glBindBuffer(GL_ARRAY_BUFFER,id[0]);
    glVertexAttribPointer(_material->pos,3,GL_FLOAT,GL_FALSE,sizeof(vertex),reinterpret_cast<const void *>(0));
    glVertexAttribPointer(_material->uv, 2,GL_FLOAT,GL_FALSE,sizeof(vertex),reinterpret_cast<const void *>(sizeof(float)*3));
    glEnableVertexAttribArray(_material->pos);
    glEnableVertexAttribArray(_material->uv);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,id[1]);
    glDrawElements(GL_TRIANGLES, indecies.size(),GL_UNSIGNED_SHORT,0); 
    check("object::render");
};

void object::color(vec::ref c)     {_color = c;}
void object::rotate(vec::ref r)    {_transform *= mat::rotate(DEG2RAD *r);}
void object::translate(vec::ref t) {_transform *= mat::translate(t);}
void object::scale(vec::ref s)     {_transform *= mat::scale(s);}
void object::position(vec::ref v)  {_transform.position(v);}  
void object::transform(mat::ref t) {_transform = t;}

vec      object::position()  const  {return _transform.position();} 
mat::ref object::transform() const  {return _transform;}
