#include "CityScapeCamera.h"
#include "AppSettings.h"

vec3 CityScapeCamera::PointWorldToScreen(const vec3 &pt) const
{
    vec3 out = pt;
    out.x *= 65.0f;
    out.y *= 47.0f;

    return out;
};