#pragma once

class vec2
{
public:
    vec2();
    vec2(float val);
    vec2(float x, float y);
    vec2(const vec2 &scnd);

    // operators

    vec2 operator = (const vec2 &scnd);

    bool operator == (const vec2 &scnd) const;
    bool operator != (const vec2 &scnd) const;
    
    vec2 operator -= (const vec2 &scnd);
    vec2 operator -= (float v);

    vec2 operator += (const vec2 &scnd);
    vec2 operator += (float v);
    
    vec2 operator *= (float v);
    vec2 operator /= (float v);

    vec2 operator - () const;
    vec2 operator - (const vec2 &scnd) const;
    vec2 operator - (float v) const;

    vec2 operator + (const vec2 &scnd) const;
    vec2 operator + (float v) const;

    vec2 operator * (float v) const;
    vec2 operator / (float v) const;

    // helpers
    float Length() const;
    float Dot(const vec2 &v) const;
    
    vec2 Normalize();
    
    bool IsZero() const;

    float x, y;

    static const vec2 ZERO;
};