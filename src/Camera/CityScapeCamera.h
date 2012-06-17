#pragma once
#include "CameraBase.h"

class CityScapeCamera : public iCamera
{
public:
      
    virtual vec3 PointWorldToScreen(const vec3 &pt) const;

    virtual void SetPos(const vec3 &pt) { m_Pos = pt; }
    virtual void Move(const vec3 &v) { m_Pos += v; }
    virtual vec3 GetPos() const { return m_Pos; }

protected:
    vec3 m_Pos;
};