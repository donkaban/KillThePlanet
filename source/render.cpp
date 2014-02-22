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

    auto mID = mat->getID();
    glUseProgram(mID);

    auto p_atr = glGetAttribLocation(mID,"pos");
    auto t_atr = glGetAttribLocation(mID,"uv");
    auto n_atr = glGetAttribLocation(mID,"normal");
    auto c_uni = glGetUniformLocation(mID,"color");
    auto t_uni = glGetUniformLocation(mID,"transform");
    auto x_uni = glGetUniformLocation(mID,"time");

    if(c_uni !=-1) glUniform4f(c_uni, color.r, color.g, color.b, color.a);
    if(t_uni !=-1) glUniformMatrix4fv(t_uni, 1,GL_FALSE,transform.data);
    if(x_uni !=-1) glUniform1f(x_uni, timer::get());

    glBindBuffer(GL_ARRAY_BUFFER,id[0]);
    glVertexAttribPointer(p_atr,3,GL_FLOAT,GL_FALSE,sizeof(vertex),reinterpret_cast<const void *>(0));
  

    glVertexAttribPointer(t_atr, 2,GL_FLOAT,GL_FALSE,sizeof(vertex),reinterpret_cast<const void *>(sizeof(float)*3));
    glVertexAttribPointer(n_atr, 3,GL_FLOAT,GL_FALSE,sizeof(vertex),reinterpret_cast<const void *>(sizeof(float)*5));    
    
    glEnableVertexAttribArray(p_atr);
    glEnableVertexAttribArray(t_atr);
    glEnableVertexAttribArray(n_atr);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,id[1]);
    glDrawElements(GL_TRIANGLES, indecies.size(),GL_UNSIGNED_SHORT,0); 
    getGLError("object::render");
 
};

object::ptr object::get() {return this->shared_from_this();}
void object::setColor(const vec &c) {color = c;}
vec  object::getColor() const       {return color;}
void object::rotate(const vec &r)    {transform *= (mat4::rotateX(r.x) * mat4::rotateY(r.y) * mat4::rotateZ(r.z));}

object::ptr object::plane(const vec &dim)
{
    float w = dim.x * .5f;
    float h = dim.y * .5f;
    object::ptr obj = std::make_shared<object>(object(
    {
         w, h, 0,  0, 1 ,  0, 0, 1, 
        -w, h, 0,  0, 0 ,  0, 0, 1, 
        -w,-h, 0,  1, 0 ,  0, 0, 1, 
         w,-h, 0,  1, 1 ,  0, 0, 1
    },
    {0,  1,  2,  2,  3,  0}));
    obj->setMaterial(std::make_shared<material>(shaders::vertex_base,shaders::fragment_base));
    obj->setColor({rnd(),rnd(),rnd(),1});
    return obj;
}
object::ptr object::cube(const vec &dim)
{
   float w = dim.x * .5f;
   float h = dim.y * .5f;
   float z = dim.z * .5f;
    object::ptr obj = std::make_shared<object>(object(
    {
        -w, -h, -z,  0, 0,   0,-1, 0, //  0  bottom
        -w, -h,  z,  0, 1,   0,-1, 0, //  1
         w, -h,  z,  1, 1,   0,-1, 0, //  2
         w, -h, -z,  1, 0,   0,-1, 0, //  3
        -w,  h, -z,  1, 0,   0, 1, 0, //  4  top
        -w,  h,  z,  1, 1,   0, 1, 0, //  5
         w,  h,  z,  0, 1,   0, 1, 0, //  6
         w,  h, -z,  0, 0,   0, 1, 0, //  7
        -w, -h, -z,  0, 0,   0, 0,-1, //  8  front
        -w,  h, -z,  0, 1,   0, 0,-1, //  9
         w,  h, -z,  1, 1,   0, 0,-1, //  10
         w, -h, -z,  1, 0,   0, 0,-1, //  11
        -w, -h,  z,  0, 0,   0, 0, 1, //  12  back
        -w,  h,  z,  0, 1,   0, 0, 1, //  13
         w,  h,  z,  1, 1,   0, 0, 1, //  14
         w, -h,  z,  1, 0,   0, 0, 1, //  15
        -w, -h, -z,  0, 0,  -1, 0, 0, //  16  left
        -w, -h,  z,  0, 1,  -1, 0, 0, //  17
        -w,  h,  z,  1, 1,  -1, 0, 0, //  18
        -w,  h, -z,  1, 0,  -1, 0, 0, //  19
         w, -h, -z,  0, 0,   1, 0, 0, //  20  right
         w, -h,  z,  0, 1,   1, 0, 0, //  21
         w,  h,  z,  1, 1,   1, 0, 0, //  22
         w,  h, -z,  1, 0,   1, 0, 0, //  23   
    },
    {
        0,  2,  1,  0,  3,  2,
        4,  5,  6,  4,  6,  7,
        8,  9,  10, 8,  10, 11,
        12, 15, 14, 12, 14, 13,
        16, 17, 18, 16, 18, 19,
        20, 23, 22, 20, 22, 21
    }));
    obj->setMaterial(std::make_shared<material>(shaders::vertex_base,shaders::fragment_base));
    return obj;
}

object::ptr object::sphere(float r, int slices)
{
    std::vector<float>    v;
    std::vector<uint16_t> n;    

    float step = (TWO_PI) / (static_cast<float>(slices));
    int   parallels = slices / 2;
    for (auto i = 0; i < parallels + 1; i++)
    {
        for (auto j = 0; j < slices + 1; j++)
        {
            auto pos_x = r * std::sin(step * static_cast<float>(i)) * std::sin(step * static_cast<float>(j));
            auto pos_y = r * std::cos(step * static_cast<float>(i));
            auto pos_z = r * std::sin(step * static_cast<float>(i)) * std::cos(step * static_cast<float>(j));
            v.insert(v.end(),{pos_x ,pos_y,pos_z ,
                         static_cast<float>(j) / static_cast<float>(slices),
                         static_cast<float>(i) / static_cast<float>(parallels), 
                         pos_x / r, pos_y / r, pos_z / r});
        }
    }    
    for (auto i = 0; i < slices / 2; i++)
    {
        for (auto j = 0; j < slices; j++)
        {
            n.push_back((i * (slices + 1) + j));
            n.push_back(((i + 1) * (slices + 1) + j));
            n.push_back(((i + 1) * (slices + 1) + (j + 1)));
            n.push_back((i * (slices + 1) + j));
            n.push_back(((i + 1) * (slices + 1) + (j + 1)));
            n.push_back((i * (slices + 1) + (j + 1)));
        }
    }
    object::ptr obj = std::make_shared<object>(v,n);
    obj->setMaterial(std::make_shared<material>(shaders::vertex_base,shaders::fragment_base));
    return obj;
}

object::ptr object::batman(const vec & dim)
{
    object::ptr obj = std::make_shared<object>(object(
    {
        0.0,-2.5, 0,  1, 0 ,   0, 0, 1,  // 0 
        2.5,-1.0, 0,  0, .5 ,  0, 0, 1,  // 1
        1.0, 0.5, 1,  1, .5 , .3, 0, .7, // 2
        0.5, 2.5, 0,  1, 1 ,   0, 0, 1,  // 3
       -1.0, 0.5, 1,  1, .5 ,-.3, 0, .7, // 4
       -2.5,-1.0, 0,  0, .5 ,  0, 0, 1,  // 5
       -0.5, 2.5, 0,  1, 1 ,   0, 0, 1,  // 6
    },
    {0,1,2,2,1,3,0,2,4,4,5,0,6,5,4,}));      

    obj->setMaterial(std::make_shared<material>(shaders::vertex_base,shaders::fragment_base));
    return obj;
}







