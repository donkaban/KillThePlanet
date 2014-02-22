#ifndef _PRECOMPILED_MODULE_
#define _PRECOMPILED_MODULE_
#include <string>

namespace shaders {

std::string vertex_base = R"(

const mat4 projection = mat4(6.00168,0.0,0.0,0.0,0.0,3.37594,0.0,0.0,0.0,0.0,-1.002,19.8398,0.0,0.0,-1.0,20.0);

attribute vec3  pos;
attribute vec2  uv;
attribute vec3  normal;
uniform   mat4  transform;
varying   vec2  v_uv;
varying   vec3  v_normal;

void main() 
{
    v_uv = uv;
    v_normal = normal;
    gl_Position = vec4(pos,1) * transform * projection;
})";


std::string fragment_base = R"(

uniform vec4 color;
uniform float time; 

varying vec2 v_uv;
varying vec3 v_normal;

void main() 
{
    vec4 col = vec4(v_uv, 0.5 + 0.5*sin(time)* 0.2 ,1.0);
    float x = v_uv.x;
    float y = v_uv.y;
    float r = -pow(abs(x*x*x*x) + abs(y*y*y*y),0.25); 
    float z = 1.0 + 0.5 * sin((r +  time * 0.25)/0.013);
    gl_FragColor = col * z;
})";


















}



#endif