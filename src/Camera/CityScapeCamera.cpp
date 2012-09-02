#include "CityScapeCamera.h"
#include "AppSettings.h"


const vec3 CityScapeCamera::RIGHT = vec3(-32.0f, 16.0f, 0.0f);
const vec3 CityScapeCamera::FORWARD = vec3(32.0f, 16.0f, 0.0f);
const vec3 CityScapeCamera::UP = vec3(0.0f, -16.0f, 0.0f);

vec3 CityScapeCamera::PointWorldToScreen(const vec3 &pt) const
{
    vec3 out = vec3::ZERO;
    
    out = RIGHT * pt.x + FORWARD * pt.z + UP * pt.y;

    out -= RIGHT * m_Pos.x + FORWARD * m_Pos.z + UP * m_Pos.y;

    return out;
};