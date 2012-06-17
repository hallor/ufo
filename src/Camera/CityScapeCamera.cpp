#include "CityScapeCamera.h"
#include "AppSettings.h"

vec3 CityScapeCamera::PointWorldToScreen(const vec3 &pt) const
{
    static vec3 RIGHT = vec3(32.0f, -16.0f, 0.0f);
    static vec3 FORWARD = vec3(-32.0f, -16.0f, 0.0f);
    static vec3 UP = vec3(0.0f, -16.0f, 0.0f);

    vec3 out = vec3::ZERO;
    
    out = RIGHT * pt.x + (-FORWARD) * pt.z + UP * pt.y;

    out -= RIGHT * m_Pos.x + (-FORWARD) * m_Pos.z + UP * m_Pos.y;

    return out;
};