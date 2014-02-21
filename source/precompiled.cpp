#ifndef _PRECOMPILED_MODULE_
#define _PRECOMPILED_MODULE_
#include <string>

namespace shaders {

std::string vertex_base = R"(

attribute vec3  pos;
attribute vec2  uv;
attribute vec3  normal;

uniform mat4 transform;

varying vec2 v_uv;
varying vec3 v_normal;

void main() 
{
    v_uv = uv;
    v_normal = normal;
    gl_Position = vec4(pos,1) * transform;
})";


std::string fragment_base = R"(

uniform vec4 color;
varying vec2 v_uv;
varying vec3 v_normal;

void main() 
{
    gl_FragColor = color;
})";


















}



#endif