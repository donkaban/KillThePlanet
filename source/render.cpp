#include "render.h"
#include "utils.h"

#include <memory>
#include <unordered_map> 
#include <cstdlib>
#include <stdexcept>


static std::unordered_map<GLenum, std::string> ERR_TABLE = 
{
    {GL_INVALID_ENUM,       "GL_INVALID_ENUM"},
    {GL_INVALID_VALUE,      "GL_INVALID_VALUE"},
    {GL_INVALID_OPERATION,  "GL_INVALID_OPERATION"},
    {GL_OUT_OF_MEMORY,      "GL_OUT_OF_MEMORY"},
    {GL_INVALID_FRAMEBUFFER_OPERATION, "GL_INVALID_FRAMEBUFFER_OPERATION"},
}; 
static void getGLError(strref where)
{
    auto error = glGetError();
    if(error !=GL_NO_ERROR)
    {
        if(ERR_TABLE.find(error) != ERR_TABLE.end())
            logger::error("OpenGL error [%s]: %s", where.c_str(),ERR_TABLE[error].c_str());
        else 
            logger::error("Unknown OpenGL error [%s]: %d", where.c_str(),error);
        exit(-1); // fatality!
    } 
        
}
static void errHandler(int id, bool shader) // :todo global common GL error handler
{
    int compiled = 0;
    if (shader)  glGetShaderiv (static_cast<GLuint>(id), GL_COMPILE_STATUS, &compiled);
    else         glGetProgramiv(static_cast<GLuint>(id), GL_LINK_STATUS,    &compiled);
    if (!compiled)
    {
        GLint message_len;
        std::string errorMessage;
        if (shader) 
        {            
            logger::error("compile shader problem! len : %d", message_len);    
            glGetShaderiv (static_cast<GLuint>(id), GL_INFO_LOG_LENGTH, &message_len);
        }
        else        
        { 
            logger::error("ink programm problem! len : %d", message_len);    
            glGetProgramiv(static_cast<GLuint>(id), GL_INFO_LOG_LENGTH, &message_len);
        }
        if (message_len > 1)
        {
            errorMessage.resize(static_cast<size_t>(message_len));
            if (shader)
            {
                glGetShaderInfoLog(id, message_len, NULL, &errorMessage[0]);
                glDeleteShader (id);
            }
            else
            {
                glGetProgramInfoLog(id, message_len, NULL, &errorMessage[0]);
                glDeleteProgram(id);
            }
            logger::error("[%s] material create: %s ",__FILE__,errorMessage.c_str());
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
    glCompileShader(vsh); errHandler(vsh, true);
    glCompileShader(fsh); errHandler(fsh, true);
  
    glAttachShader(id, vsh);
    glAttachShader(id, fsh);
    glLinkProgram(id); errHandler(id,false);
    getGLError("material::bind");
    pos  = glGetAttribLocation(id,"pos");
    uv   = glGetAttribLocation(id,"uv");
    col  = glGetUniformLocation(id,"color");
    mv   = glGetUniformLocation(id,"mView");
    time = glGetUniformLocation(id,"time");

}
void material::unbind() {}
   
GLuint material::getID() const {return id;}


object::object(const std::vector<float> &v, const std::vector<uint16_t> &ndx) :
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
    getGLError("object::bind");
}
void object::unbind() 
{
    glDeleteBuffers(2,id);
    getGLError("object::unbind");
} 

void object::setMaterial(material::ref m) {mat = m;}

void object::render() 
{
    if(!mat) return;
    glUseProgram(mat->getID());

    if(mat->col  !=-1) glUniform4f(mat->col, col.r, col.g, col.b, 1);
    if(mat->mv   !=-1) glUniformMatrix4fv(mat->mv, 1,GL_FALSE,_transform.data);
    if(mat->time !=-1) glUniform1f(mat->time, timer::get());

    glBindBuffer(GL_ARRAY_BUFFER,id[0]);
    glVertexAttribPointer(mat->pos,3,GL_FLOAT,GL_FALSE,sizeof(vertex),reinterpret_cast<const void *>(0));
    glVertexAttribPointer(mat->uv,2,GL_FLOAT,GL_FALSE,sizeof(vertex),reinterpret_cast<const void *>(sizeof(float)*3));
    glEnableVertexAttribArray(mat->pos);
    glEnableVertexAttribArray(mat->uv);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,id[1]);
    glDrawElements(GL_TRIANGLES, indecies.size(),GL_UNSIGNED_SHORT,0); 
    getGLError("object::render");
 
};

void object::color(const vec &c)        {col = c;}
void object::rotate(const vec &r)       {_transform *= mat4::rotate(DEG2RAD *r);}
void object::translate(const vec &t)    {_transform *= mat4::translate(t);}
void object::scale(const vec &s)        {_transform *= mat4::scale(s);}
void object::position(const vec &v)     {_transform.position(v);}  
vec  object::position() const           {return _transform.position();} 
void object::transform(const mat4 &t)   {_transform = t;}
const mat4 & object::transform() const  {return _transform;}

object::ptr object::cube(const vec &dim)
{
   float w = dim.x * .5f;
   float h = dim.y * .5f;
   float z = dim.z * .5f;
    object::ptr obj = std::make_shared<object>(object(
    {
        -w,-h,-z, 0, 0,-w,-h, z, 0, 1, w,-h, z, 1, 1, w,-h,-z, 1, 0,
        -w, h,-z, 1, 0,-w, h, z, 1, 1, w, h, z, 0, 1, w, h,-z, 0, 0,
        -w,-h,-z, 0, 0,-w, h,-z, 0, 1, w, h,-z, 1, 1, w,-h,-z, 1, 0,
        -w,-h, z, 0, 0,-w, h, z, 0, 1, w, h, z, 1, 1, w,-h, z, 1, 0,
        -w,-h,-z, 0, 0,-w,-h, z, 0, 1,-w, h, z, 1, 1,-w, h,-z, 1, 0, 
         w,-h,-z, 0, 0, w,-h, z, 0, 1, w, h, z, 1, 1, w, h,-z, 1, 0,    
    },
    {
        0,  2,  1,  0,  3,  2,
        4,  5,  6,  4,  6,  7,
        8,  9,  10, 8,  10, 11,
        12, 15, 14, 12, 14, 13,
        16, 17, 18, 16, 18, 19,
        20, 23, 22, 20, 22, 21
    }));
    return obj;
}
