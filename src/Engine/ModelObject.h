#pragma once
#include "GameObject.h"

class iModelObjectVis;

class iModelObject : public iGameObject
{
    DECLARE_GAME_OBJECT_ABSTRACT(iModelObject, iGameObject);

public:

    iModelObject();

    virtual void OnCreate();
    virtual void OnDestroy();

    virtual void OnRenderFrame();

    virtual iModelObjectVis* GetVis() const { return m_Visualisation; };

    // Position in world space
    virtual vec3 GetPos() const { return m_Pos; };
    virtual void SetPos(const vec3 &pos) { m_Pos = pos; };

    virtual vec3 GetWantedPos() const { return m_WantedPos; };
    virtual void SetWantedPos(const vec3 &pos) { m_WantedPos = pos; }

protected:

    virtual iModelObjectVis* CreateVis() = 0;
    virtual void DestroyVis();
    
private:

    vec3 m_Pos;
    vec3 m_WantedPos;
    
    iModelObjectVis *m_Visualisation;
};