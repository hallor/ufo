#pragma once
#include "CameraBase.h"

class CityScapeCamera : public iCamera
{
public:
      
    virtual vec3 PointWorldToScreen(const vec3 &pt) const;
};