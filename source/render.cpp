#include "render.h"
#include "utils.h"

#include <unordered_map> 

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
material::material(strref tag, strref vsh, strref fsh) : 
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
        v_shader = "#define GLES" + v_shader;
        f_shader = "#define GLES" + f_shader;
    #endif

    id = glCreateProgram(); 

    auto vsh = glCreateShader(GL_VERTEX_SHADER);    
    auto fsh = glCreateShader(GL_FRAGMENT_SHADER);  
    if(vsh == 0 || fsh == 0)
        throw std::runtime_error("can't create shader, perhaps GL context not created");
    const char *v_src = v_shader.c_str();
    const char *f_src = f_shader.c_str();
    
    glShaderSource(vsh, 1, &v_src, NULL); 
    glShaderSource(fsh, 1, &f_src, NULL); 
    glCompileShader(vsh); errHandler(vsh, true);
    glCompileShader(fsh); errHandler(fsh, true);
  
    glAttachShader(id, vsh);
    glAttachShader(id, fsh);
    glLinkProgram(id); errHandler(id,false);
    getGLError("material::bind");
}
void material::unbind() {}
   
GLuint material::getID() const {return id;}


object::object(const std::vector<vertex> &v, const std::vector<uint16_t> &ndx) :
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
    glBufferData(GL_ARRAY_BUFFER, vertexes.size() * sizeof(vertex) , &vertexes[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,id[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indecies.size() * sizeof(float), &indecies[0], GL_STATIC_DRAW);
    getGLError("object::bind");
}
void object::unbind() {} 

void object::setMaterial(material::ref m) {mat = m;}

void object::render() 
{
    if(!mat) return;

    auto p_ndx = glGetAttribLocation(mat->getID(),"position");
    auto t_ndx = glGetAttribLocation(mat->getID(),"uv");
    auto n_ndx = glGetAttribLocation(mat->getID(),"normal");
    
    auto c_uni = glGetUniformLocation(mat->getID(),"color");
    auto p_uni = glGetUniformLocation(mat->getID(),"position");
    auto r_uni = glGetUniformLocation(mat->getID(),"rotation");

    glUseProgram(mat->getID());

    if(c_uni !=-1) glUniform4f(c_uni, color.r, color.g, color.b, color.a);
    if(p_uni !=-1) glUniform3f(p_uni, position.x, position.y, position.z);
    if(r_uni !=-1) glUniform3f(r_uni, rotation.x, rotation.y, rotation.z);

    glBindBuffer(GL_ARRAY_BUFFER,id[0]);
   
    if(p_ndx !=-1) 
    {
        glVertexAttribPointer(p_ndx, 3 ,GL_FLOAT, GL_FALSE, sizeof(vertex), (const void *) 0);
        glEnableVertexAttribArray(p_ndx);
    }
    if(t_ndx !=-1) 
    {
        glVertexAttribPointer(t_ndx, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (const void *) (sizeof(float) * 3));
        glEnableVertexAttribArray(t_ndx);
    }
    if(n_ndx !=-1) 
    {
        glVertexAttribPointer(n_ndx, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (const void *) (sizeof(float) * 5));    
        glEnableVertexAttribArray(n_ndx);
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,id[1]);
    glDrawElements(GL_TRIANGLES, sizeof(indecies)/sizeof(uint16_t),GL_UNSIGNED_SHORT,0); 
    getGLError("object::render");
 
};

object::ptr object::get() {return this->shared_from_this();}















