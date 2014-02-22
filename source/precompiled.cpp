#ifndef _PRECOMPILED_MODULE_
#define _PRECOMPILED_MODULE_
#include <string>

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
    gl_FragColor = color;
})";



















}



#endif