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

class vec3
{
public:
    vec3();
    vec3(float val);
    vec3(float x, float y, float z);
    vec3(const vec3 &scnd);

    // operators

    vec3 operator = (const vec3 &scnd);

    bool operator == (const vec3 &scnd) const;
    bool operator != (const vec3 &scnd) const;
    
    vec3 operator -= (const vec3 &scnd);
    vec3 operator -= (float v);

    vec3 operator += (const vec3 &scnd);
    vec3 operator += (float v);
    
    vec3 operator *= (float v);
    vec3 operator /= (float v);

    vec3 operator - () const;
    vec3 operator - (const vec3 &scnd) const;
    vec3 operator - (float v) const;

    vec3 operator + (const vec3 &scnd) const;
    vec3 operator + (float v) const;

    vec3 operator * (float v) const;
    vec3 operator / (float v) const;

    // helpers
    float Length() const;
    float Length2() const;
    float Dot(const vec3 &v) const;
    vec3  Cross(const vec3 &v) const;
    
    float Volume() const;

    vec3 Normalize();
    
    bool IsZero() const;

    float GetWidth() const { return x; }
    float GetHeight() const { return y; }
    float GetBreadth() const { return z; }

    float x, y, z;

    static const vec3 ZERO;
};