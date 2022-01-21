#pragma once
#include "../Libraries/box2d/include/box2d/box2d.h"

namespace fw {

class vec2
{
public:
    vec2() {}
    vec2(float nx, float ny) { x = nx; y = ny; }

    static void Test();

    void Set(float nx, float ny) { x = nx; y = ny; }

    vec2 operator+(const vec2& o) const { return vec2( x + o.x, y + o.y ); }
    vec2 operator-(const vec2& o) const { return vec2( x - o.x, y - o.y ); }
    vec2 operator*(const vec2& o) const { return vec2( x * o.x, y * o.y ); }
    vec2 operator/(const vec2& o) const { return vec2( x / o.x, y / o.y ); }

    vec2 operator*(float o) const { return vec2( x * o, y * o ); }
    vec2 operator/(float o) const { return vec2( x / o, y / o ); }

    vec2& operator+=(const vec2& o) { x += o.x; y += o.y; return *this; }
    vec2& operator-=(const vec2& o) { x -= o.x; y -= o.y; return *this; }
    vec2& operator*=(const vec2& o) { x *= o.x; y *= o.y; return *this; }
    vec2& operator/=(const vec2& o) { x /= o.x; y /= o.y; return *this; }

    vec2 operator-() { return vec2(-x, -y); }

    bool operator==(const vec2& o) const { return (x == o.x && y == o.y); }
    bool operator!=(const vec2& o) const { return (x != o.x || y != o.y); }

    float Length() const
    {
        return sqrtf( x*x + y*y );
    }

    float DistanceTo(const vec2& o) const
    {
        return (*this - o).Length();
    }

    void Normalize()
    {
        if( x != 0 || y != 0 )
        {
            float len = Length();
            x /= len;
            y /= len;
        }
    }

    vec2 GetNormalized() const
    {
        if( x != 0 || y != 0 )
        {
            float len = Length();
            return *this / len;
        }

        return vec2(0,0);
    }

    float Dot(const vec2& o) const
    {
        return x*o.x + y*o.y;
    }

    operator b2Vec2() const { return b2Vec2(x, y); }

public:
    float x = 0;
    float y = 0;
};

inline vec2 operator*(float f, const vec2& v) { return vec2( v.x * f, v.y * f ); }
inline vec2 operator/(float f, const vec2& v) { return vec2( v.x / f, v.y / f ); }

class ivec2
{
public:
    ivec2() {}
    ivec2(int nx, int ny) { x = nx; y = ny; }

    void Set(int nx, int ny) { x = nx; y = ny; }

    //vec2 operator+(const vec2& o) const { return vec2( x + o.x, y + o.y ); }
    //vec2 operator-(const vec2& o) const { return vec2( x - o.x, y - o.y ); }
    //vec2 operator*(const vec2& o) const { return vec2( x * o.x, y * o.y ); }
    //vec2 operator/(const vec2& o) const { return vec2( x / o.x, y / o.y ); }

    //vec2 operator*(float o) const { return vec2( x * o, y * o ); }
    //vec2 operator/(float o) const { return vec2( x / o, y / o ); }

    //vec2& operator+=(const vec2& o) { x += o.x; y += o.y; return *this; }
    //vec2& operator-=(const vec2& o) { x -= o.x; y -= o.y; return *this; }
    //vec2& operator*=(const vec2& o) { x *= o.x; y *= o.y; return *this; }
    //vec2& operator/=(const vec2& o) { x /= o.x; y /= o.y; return *this; }

    //vec2 operator-() { return vec2(-x, -y); }

    //bool operator==(const vec2& o) const { return (x == o.x && y == o.y); }
    //bool operator!=(const vec2& o) const { return (x != o.x || y != o.y); }

public:
    int x = 0;
    int y = 0;
};

} // namespace fw
