#include "render.h"

std::unordered_map<std::string,material::ptr> material::cache;

material::material() {}
material::material(strref tag, strref vsh, strref fsh) : 
    v_shader(vsh),
    f_shader(fsh)
{
    bind();
    cache[tag] = this->shared_from_this();
}
material::~material()
{}

void material::bind()   
{
    id = glCreateProgram(); 
    auto vsh = glCreateShader(GL_VERTEX_SHADER);    
    auto fsh = glCreateShader(GL_FRAGMENT_SHADER);  
    if(vsh == 0 || fsh == 0)
        throw std::runtime_error("can't create shader, perhaps GL context not created");
    const char *v_src = v_shader.c_str();
    const char *f_src = f_shader.c_str();
    glShaderSource(vsh, 1, &v_src, NULL); 
    glShaderSource(fsh, 1, &f_src, NULL); 
    glCompileShader(vsh); 
    glCompileShader(fsh); 
    glAttachShader(id, vsh);
    glAttachShader(id, fsh);
    glLinkProgram(id);
}
void material::unbind() {}
   
GLuint material::getID() const {return id;}
material::ptr material::get(strref tag) {return cache[tag];}


object::object(const std::vector<vertex> &v, const std::vector<uint16_t> &i) :
        vertexes(v),
        indecies(i) 
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indecies.size(), &indecies[0],GL_STATIC_DRAW);
}
void object::unbind() {} 

void object::setMaterial(material::ref m) {mat = m;}

void object::render() 
{
    if(!mat) return;
    glUseProgram(mat->getID());
    glBindBuffer(GL_ARRAY_BUFFER,id[0]);
    //glVertexAttribPointer(pos_ndx, 4 ,GL_FLOAT, GL_FALSE, sizeof(vertex) / 4 , (const void *) 0);
    //glVertexAttribPointer(tcc_ndx, 4, GL_FLOAT, GL_FALSE, sizeof(vertex) / 4,  (const void *) (sizeof(float) * 4));
    //glEnableVertexAttribArray(pos_ndx);
    //glEnableVertexAttribArray(tcc_ndx);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,id[1]);
    glDrawElements(GL_TRIANGLES, sizeof(indecies)/sizeof(uint16_t),GL_UNSIGNED_SHORT,0); 
};

object::ptr object::get() {return this->shared_from_this();}