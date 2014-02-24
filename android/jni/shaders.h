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

std::string back = R"(
uniform vec4 color;
uniform float time; 
varying vec2 v_uv;
void main() 
{
    float yamp=.1/(0.001 + abs(sin(-2.0*v_uv.y+0.960+0.75*sin(time)))+abs(sin(5.0*v_uv.x+2.42) + abs(sin(4.0*v_uv.y-1.92)) ) );
    gl_FragColor = vec4( yamp/7. , yamp/5.,  .2+yamp ,.5 ); 
})";

std::string enemy = R"(
uniform vec4 color;
uniform float time; 
varying vec2 v_uv;

vec4 rnd(vec2 uv)
{
    uv = floor(fract(uv)*1e3);
    float v = uv.x+uv.y*1e3;
    return fract(1e5*sin(vec4(v*1e-2, (v+1.)*1e-2, (v+1e3)*1e-2, (v+1e3+1.)*1e-2)));
}
 
float noise(vec2 p) 
{
    vec2 f = fract(p*1e3);
    vec4 r = rnd(p);
    f = f*f*(3.0-2.0*f);
    return (mix(mix(r.x, r.y, f.x), mix(r.z, r.w, f.x), f.y));  
}
float col(vec2 p) 
{
    float v = 0.0;
    v += 1.-abs(pow(noise(p)-0.5,0.75))*1.7;
    return v;
}
void main() 
{
    vec2 p = v_uv *.02 +.05;
    float c1 = col(p*.3+time*.0015);
    float c2 = col(p*.3-time*.0015);
    
    float c3 = col(p*.2-time*.0015);
    float c4 = col(p*.2+time*.0015);
    
    float cf = pow(c1*c2*c3*c4+0.5,6.);
    
    vec3 c = vec3(cf);
    gl_FragColor = vec4(c+vec3(0.2,0.2,.6), 1.);
})";
}
