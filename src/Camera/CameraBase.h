#pragma once
#include "vec.h"

class iCamera
{
public:
    virtual ~iCamera() {};

    virtual vec3 PointWorldToScreen(const vec3 &pt) const = 0;
};