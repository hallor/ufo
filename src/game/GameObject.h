#pragma once
#include "vec.h"

class iGameObjectVis;

class iGameObject
{
public:
    iGameObject();
    virtual ~iGameObject();

    virtual void OnCreate();
    virtual void OnDestroy();

    virtual iGameObjectVis* GetVis() const { return m_Visualisation; };

    // Position in world space
    virtual vec3 GetPos() const { return m_Pos; };
    virtual void SetPos(const vec3 &pos) { m_Pos = pos; };
    
    // Position in world space
    virtual vec3 GetWantedPos() const { return m_WantedPos; };
    virtual void SetWantedPos(const vec3 &pos) { m_WantedPos = pos; }

    virtual float GetSpeed() const { return m_Speed; }
    virtual void  SetSpeed(float speed) { m_Speed = speed; }

    // Rememver to call UpdateVisualisation whenever needed
    virtual void Update(float dt) { UpdateVisualisation(dt); };
    virtual void UpdateVisualisation(float dt);

protected:
    virtual iGameObjectVis* CreateVis() = 0;
    virtual void DestroyVis();

private:

    vec3 m_Pos;
    float m_Speed;

    vec3 m_WantedPos;
    
    iGameObjectVis *m_Visualisation;
};