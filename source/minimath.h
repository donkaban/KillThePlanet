#ifndef _MATH_MODULE_
#define _MATH_MODULE_

#include <cmath> 
#include <cstdlib>
#include <initializer_list>
 
extern float rnd();

class vec4 
{
public:
    union
    {
        struct { float x, y, z, w; };
        struct { float r, g, b, a; };
    };
       
    vec4() : x(0),y(0),z(0),w(0) {}
    inline explicit vec4(const float _x, const float _y,const float _z, const float _w = 1) : 
    	x(_x), y(_y),z(_z), w(_w)  
    {}

    inline vec4  operator- () const {return vec4(-x,-y,-z,-w);}
    inline float operator* (const vec4 &a) const {return x * a.x + y * a.y + z * a.z + w * a.w;}
    inline vec4  operator* (const float a) const {return vec4( x * a, y * a, z * a, w * a);}
    inline vec4  operator+ (const vec4 &a) const {return vec4(x + a.x, y + a.y, z+ a.z, w + a.w);}
    inline vec4  operator- (const vec4 &a) const {return vec4(x - a.x, y - a.y, z- a.z, w - a.w);}

    inline vec4 & operator+= (const vec4 &a) {x += a.x; y += a.y; z += a.z; w += a.w; return *this;}
    inline vec4 & operator-= (const vec4 &a) {x -= a.x; y -= a.y; z -= a.z; w -= a.w; return *this;}
    inline vec4 & operator/= (const vec4 &a) {x /= a.x; y /= a.y; z /= a.z; w /= a.w; return *this;}
    inline vec4 & operator*= (const float a){x *= a; y *= a; z *= a; w *= a; return *this; }
   
    inline bool compare(const vec4 &a) const {return ((x == a.x) && (y == a.y) && (z == a.z) && (w == a.w));}
    inline bool operator== (const vec4 &a) const {return compare(a);}
    inline bool operator!= (const vec4 &a) const {return !compare(a);}
    
    inline float length()  const {return std::sqrt(x * x + y * y + z * z + w * w);}

    inline friend vec4 operator* (const float a, const vec4 b) {return vec4( b.x * a, b.y * a, b.z * a, b.w * a);}
    inline const float * ptr() const {return &x;}
    inline float       * ptr()       {return &x;}
   
};






#endif