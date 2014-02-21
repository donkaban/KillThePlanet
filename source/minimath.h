#ifndef _MATH_MODULE_
#define _MATH_MODULE_

#include <cmath> 
#include <cstring>
#include <initializer_list>
 
extern float rnd();

template<typename T> bool between(const T &v, const T &v0, const T &v1)
{
    return (v >= v0 && v <= v1);
}     
template<typename T> T lerp(const T &v0,const T &v1,float u) 
{
    return v0 + (v1 - v0) * u;
}

struct vec 
{
public:
    union
    {
        struct { float x, y, z, w; };
        struct { float r, g, b, a; };
        float data[4];
    };
       
    vec(const float _x = 0, const float _y = 0 ,const float _z = 0, const float _w = 0) : 
    	x(_x), y(_y),z(_z), w(_w)  
    {}
    vec(std::initializer_list<float> l) 
    {
        std::memcpy(data,l.begin(),sizeof(data));
    }   

    vec   operator-  () const {return vec(-x,-y,-z,-w);}
    float operator*  (const vec &a)  const {return x * a.x + y * a.y + z * a.z + w * a.w;}
    vec   operator*  (const float a) const {return vec( x * a, y * a, z * a, w * a);}
    vec   operator+  (const vec &a)  const {return vec(x + a.x, y + a.y, z+ a.z, w + a.w);}
    vec   operator-  (const vec &a)  const {return vec(x - a.x, y - a.y, z- a.z, w - a.w);}

    vec & operator+= (const vec &a)  {x += a.x; y += a.y; z += a.z; w += a.w; return *this;}
    vec & operator-= (const vec &a)  {x -= a.x; y -= a.y; z -= a.z; w -= a.w; return *this;}
    vec & operator/= (const vec &a)  {x /= a.x; y /= a.y; z /= a.z; w /= a.w; return *this;}
    vec & operator*= (const float a) {x *= a; y *= a; z *= a; w *= a; return *this; }
   
    
    bool operator== (const vec &a) const {return compare(a);}
    bool operator!= (const vec &a) const {return !compare(a);}
    
    bool  compare(const vec &a) const {return ((x == a.x) && (y == a.y) && (z == a.z) && (w == a.w));}
    float length()  const {return std::sqrt(x * x + y * y + z * z + w * w);}
    
    friend vec operator* (const float a, const vec b) {return vec( b.x * a, b.y * a, b.z * a, b.w * a);}
};

struct mat4
{
    union
    {
        float   mat[4][4];
        float   data[16];
    };

    mat4() : mat4({1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}) {}
    mat4(std::initializer_list<float> l) 
    {
        std::memcpy(data,l.begin(),sizeof(data));
    }   
    
    mat4 operator* (const mat4 &a) const
    {
        mat4   dst;
        const float *m1  = reinterpret_cast<const float *>(this);
        const float *m2  = reinterpret_cast<const float *>(&a);
        float *ptr = reinterpret_cast<float *>(&dst);
        for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                *ptr = m1[0] * m2[j]
                    +  m1[1] * m2[j + 4 ]
                    +  m1[2] * m2[j + 8 ]
                    +  m1[3] * m2[j + 12];
                ptr++;
            }
            m1 += 4;
        }
        return dst;
    }
    mat4 & operator*= (const mat4 &a)
    {
        *this = (*this) * a;
        return *this;
    }
    static mat4 rotateZ(float angle)
    {
        auto s = std::sin(angle);
        auto c = std::cos(angle);
        return {c,-s,0,0,s,c,0,0,0,0,1,0,0,0,0,1};
    }
    inline static mat4 rotateX(float angle)
    {
        auto s = std::sin(angle);
        auto c = std::cos(angle);
        return {1,0,0,0,0,c,-s,0,0,s,c,0,0,0,0,1};
    }

    inline static mat4 rotateY(float angle)
    {
        auto s = std::sin(angle);
        auto c = std::cos(angle);
        return {c,0,s,0,0,1,0,0,-s,0,c,0,0,0,0,1};
    }


};




#endif