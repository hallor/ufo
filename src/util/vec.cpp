#include "game_pch.h"
#include "vec.h"
#include <math.h>

///////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////2 Dim vector//////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
const vec2 vec2::ZERO = vec2(0.0f);

vec2::vec2()
{
    x = y = 0.0f;
}

vec2::vec2(float v)
{
    x = y = v;
}

vec2::vec2(float _x, float _y)
{
    x = _x;
    y = _y;
}

vec2::vec2(const vec2 &scnd)
{
    if(&scnd == this)
        return;

    x = scnd.x;
    y = scnd.y;
}

vec2 vec2::operator = (const vec2 &scnd)
{
    if(&scnd == this)
        return *this;

    x = scnd.x;
    y = scnd.y;

    return *this;
}

bool vec2::operator == (const vec2 &scnd) const
{
    return x == scnd.x && y == scnd.y;
};

bool vec2::operator != (const vec2 &scnd) const
{
    return !(*this == scnd);
};

vec2 vec2::operator -= (const vec2 &scnd)
{
    x -= scnd.x;
    y -= scnd.y;

    return *this;
};

vec2 vec2::operator -= (float v)
{
    x -= v;
    y -= v;

    return *this;
};

vec2 vec2::operator += (const vec2 &scnd)
{
    x += scnd.x;
    y += scnd.y;

    return *this;
};

vec2 vec2::operator += (float v)
{
    x += v;
    y += v;

    return *this;
};
    
vec2 vec2::operator *= (float v)
{
    x *= v;
    y *= v;

    return *this;
};

vec2 vec2::operator /= (float v)
{
    x /= v;
    y /= v;

    return *this;
};

vec2 vec2::operator - () const
{
    vec2 tmp(-x, -y);

    return tmp;
};

vec2 vec2::operator - (const vec2 &scnd) const
{
    vec2 tmp(x, y);
    tmp -= scnd;

    return tmp;
};

vec2 vec2::operator - (float v) const
{
    vec2 tmp(x, y);
    tmp -= v;

    return tmp;
};

vec2 vec2::operator + (const vec2 &scnd) const
{
    vec2 tmp(x, y);
    tmp += scnd;

    return tmp;
};

vec2 vec2::operator + (float v) const
{
    vec2 tmp(x, y);
    tmp += v;

    return tmp;
};

vec2 vec2::operator * (float v) const
{
    vec2 tmp(x, y);
    tmp *= v;

    return tmp;
};

vec2 vec2::operator / (float v) const
{
    vec2 tmp(x, y);
    tmp /= v;

    return tmp;
};

float vec2::Length() const
{
    return sqrt(x * x + y * y);
};

float vec2::Dot(const vec2 &v) const
{
    return x * v.x + y * v.y;
};

vec2 vec2::Normalize()
{
    float lgt = Length();

    if(IsZero())
        return *this;

    x /= lgt;
    y /= lgt;

    return *this;
};

bool vec2::IsZero() const
{
    return Length() < 0.0001f;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////3 Dim vector//////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
const vec3 vec3::ZERO = vec3(0.0f);

vec3::vec3()
{
    x = y = z = 0.0f;
}

vec3::vec3(float v)
{
    x = y = z = v;
}

vec3::vec3(float _x, float _y, float _z)
{
    x = _x;
    y = _y;
    z = _z;
}

vec3::vec3(const vec3 &scnd)
{
    if(&scnd == this)
        return;

    x = scnd.x;
    y = scnd.y;
    z = scnd.z;
}

vec3 vec3::operator = (const vec3 &scnd)
{
    if(&scnd == this)
        return *this;

    x = scnd.x;
    y = scnd.y;
    z = scnd.z;

    return *this;
}

bool vec3::operator == (const vec3 &scnd) const
{
    return x == scnd.x && y == scnd.y && z == scnd.z;
};

bool vec3::operator != (const vec3 &scnd) const
{
    return !(*this == scnd);
};

vec3 vec3::operator -= (const vec3 &scnd)
{
    x -= scnd.x;
    y -= scnd.y;
    z -= scnd.z;

    return *this;
};

vec3 vec3::operator -= (float v)
{
    x -= v;
    y -= v;
    z -= v;

    return *this;
};

vec3 vec3::operator += (const vec3 &scnd)
{
    x += scnd.x;
    y += scnd.y;
    z += scnd.z;

    return *this;
};

vec3 vec3::operator += (float v)
{
    x += v;
    y += v;
    z += v;

    return *this;
};
    
vec3 vec3::operator *= (float v)
{
    x *= v;
    y *= v;
    z *= v;

    return *this;
};

vec3 vec3::operator /= (float v)
{
    x /= v;
    y /= v;
    z /= v;

    return *this;
};

vec3 vec3::operator - () const
{
    vec3 tmp(-x, -y, -z);

    return tmp;
};

vec3 vec3::operator - (const vec3 &scnd) const
{
    vec3 tmp(x, y, z);
    tmp -= scnd;

    return tmp;
};

vec3 vec3::operator - (float v) const
{
    vec3 tmp(x, y, z);
    tmp -= v;

    return tmp;
};

vec3 vec3::operator + (const vec3 &scnd) const
{
    vec3 tmp(x, y, z);
    tmp += scnd;

    return tmp;
};

vec3 vec3::operator + (float v) const
{
    vec3 tmp(x, y, z);
    tmp += v;

    return tmp;
};

vec3 vec3::operator * (float v) const
{
    vec3 tmp(x, y, z);
    tmp *= v;

    return tmp;
};

vec3 vec3::operator / (float v) const
{
    vec3 tmp(x, y, z);
    tmp /= v;

    return tmp;
};

float vec3::Length() const
{
    return sqrt(x * x + y * y + z * z);
};

float vec3::Length2() const
{
    return x * x + y * y + z * z;
}

float vec3::Length_2D() const
{
    return sqrt(x * x + z * z);
};

float vec3::Length2_2D() const
{
    return x * x + z * z;
}

float vec3::Dot(const vec3 &v) const
{
    return x * v.x + y * v.y + z * v.z;
};

vec3 vec3::Cross(const vec3 &v) const
{
    return (x - z) * v.y + (z - y) * v.x + (y - x) * v.z;
};

float vec3::Volume() const
{
    return x * y * z;
}

vec3 vec3::Normalize()
{
    float lgt = Length();

    if(IsZero())
        return *this;

    x /= lgt;
    y /= lgt;
    z /= lgt;

    return *this;
};

bool vec3::IsZero() const
{
    return Length() < 0.0001f;
}
