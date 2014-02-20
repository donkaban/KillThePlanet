#ifndef _MATH_MODULE_
#define _MATH_MODULE_

#include <cmath> 
#include <cstdlib>
#include <initializer_list>
 
extern float rnd();

class vec 
{
public:
    union
    {
        struct { float x, y, z, w; };
        struct { float r, g, b, a; };
    };
       
    vec() : x(0),y(0),z(0),w(0) {}
    inline explicit vec(const float _x, const float _y,const float _z = 0, const float _w = 1) : 
    	x(_x), y(_y),z(_z), w(_w)  
    {}

    inline vec  operator- () const {return vec(-x,-y,-z,-w);}
    inline float operator* (const vec &a) const {return x * a.x + y * a.y + z * a.z + w * a.w;}
    inline vec  operator* (const float a) const {return vec( x * a, y * a, z * a, w * a);}
    inline vec  operator+ (const vec &a) const {return vec(x + a.x, y + a.y, z+ a.z, w + a.w);}
    inline vec  operator- (const vec &a) const {return vec(x - a.x, y - a.y, z- a.z, w - a.w);}

    inline vec & operator+= (const vec &a) {x += a.x; y += a.y; z += a.z; w += a.w; return *this;}
    inline vec & operator-= (const vec &a) {x -= a.x; y -= a.y; z -= a.z; w -= a.w; return *this;}
    inline vec & operator/= (const vec &a) {x /= a.x; y /= a.y; z /= a.z; w /= a.w; return *this;}
    inline vec & operator*= (const float a){x *= a; y *= a; z *= a; w *= a; return *this; }
   
    inline bool compare(const vec &a) const {return ((x == a.x) && (y == a.y) && (z == a.z) && (w == a.w));}
    inline bool operator== (const vec &a) const {return compare(a);}
    inline bool operator!= (const vec &a) const {return !compare(a);}
    
    inline float length()  const {return std::sqrt(x * x + y * y + z * z + w * w);}

    inline friend vec operator* (const float a, const vec b) {return vec( b.x * a, b.y * a, b.z * a, b.w * a);}
    inline const float * ptr() const {return &x;}
    inline float       * ptr()       {return &x;}
   
};






#endif