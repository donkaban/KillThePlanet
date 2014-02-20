#ifndef _PRECOMPILED_MODULE_
#define _PRECOMPILED_MODULE_
#include <string>

namespace shaders {

std::string vertex_base = R"(
#ifdef GLES
	precision highp float;
#endif

attribute vec3  position;
attribute vec2  uv;
attribute vec3  normal;

varying vec2 v_uv;
varying vec3 v_normal;

void main() 
{
	v_uv = uv;
	v_normal = normal;
    gl_Position = vec4(position,1);
})";


std::string fragment_base = R"(
#ifdef GLES
	precision highp float;
#endif

uniform vec4 color;
varying vec2 v_uv;
varying vec3 v_normal;

void main() 
{
	gl_FragColor = color;
})";


















}



#endif