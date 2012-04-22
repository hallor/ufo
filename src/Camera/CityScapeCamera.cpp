#include "CityScapeCamera.h"
#include "AppSettings.h"

vec3 CityScapeCamera::PointWorldToScreen(const vec3 &pt) const
{
    vec3 out = pt;
    out.x *= AppSettings::GetWindowWidth() / 1920.0f;
    out.y *= AppSettings::GetWindowHeight() / 1080.0f;

    return out;
};