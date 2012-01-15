#include "vec.h"
#include <math.h>

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
