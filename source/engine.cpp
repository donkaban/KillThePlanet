#include "engine.h"
#include <cstdlib>

namespace shaders {

std::string vertex_base = R"(
const mat4 projection = mat4(6.00168,0.0,0.0,0.0,0.0,3.37594,0.0,0.0,0.0,0.0,-1.002,19.8398,0.0,0.0,-1.0,20.0);

attribute vec3  pos;
attribute vec2  uv;
uniform   mat4  mView;
varying   vec2  v_uv;

void main() 
{
    v_uv = uv;
    gl_Position = vec4(pos,1) * mView * projection;
})";

std::string ship = R"(
uniform vec4 color;
uniform float time; 
varying vec2 v_uv;

void main() 
{
    vec4 col =  vec4(v_uv,sin(time*5.),1.0);
    float x = pow(v_uv.x,4.0);
    float y = pow(v_uv.y,4.0);
    float r = pow(x + y,0.25); 
    float z = 1.0 + sin((r+time *.5)*45.0);
    gl_FragColor = col * z;
})";


std::string bullet = R"(
uniform vec4 color;
uniform float time; 
varying vec2 v_uv;

void main() 
{
    gl_FragColor = color* vec4(v_uv,1.0,1.0);
})";
}


std::unordered_map<std::string, material::ptr> engine::materials;
 
void engine::_update()
{
 	auto currTime = timer::get();
 	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    if(onUpdate) onUpdate(currTime-deltaTime);
    deltaTime = currTime;
}

void engine::_init()
{
    srand (time(NULL));
  	glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
	glEnable (GL_BLEND);
    glDepthFunc(GL_LEQUAL); 
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);    
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    logger::info("create materials");
    materials["ship"]   = std::make_shared<material>(shaders::vertex_base,shaders::ship);
    materials["bullet"] = std::make_shared<material>(shaders::vertex_base,shaders::bullet);
}

