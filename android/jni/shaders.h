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
varying vec2 v_uv;
void main() 
{
    gl_FragColor = vec4(1,1,1,1);
})";

std::string back = R"(
uniform float time; 
varying vec2 v_uv;
void main() 
{
    float yamp=.1/(0.001 + abs(sin(-2.0*v_uv.y+0.960+0.75*sin(time)))+abs(sin(5.0*v_uv.x+2.42) + abs(sin(4.0*v_uv.y-1.92)) ) );
    gl_FragColor = vec4( yamp/7. , yamp/5.,  .2+yamp ,.5 ); 
})";

std::string enemy = R"(
uniform float time; 
varying vec2 v_uv;
float rand(vec3 v)
{
    return fract(sin(dot(v*0.123,vec3(12.9898,78.233,112.166))) * 43758.5453);
}
float fbm(vec3 v) 
{
    float n = 0.0;
    n += 0.5000 * rand(v); v *= 2.1;
    n += 0.2500 * rand(v); v *= 2.1;
    
    return n;
}
void main() 
{
    float color = fbm(vec3(v_uv,0.0));
    gl_FragColor = vec4(color,color,color,1.0);
})";
}
