#pragma once
#include <string>
#include "Texture.h"
#include "GameObject.h"

class iModelObject;
class cSprite3D;

class iModelObjectVis
{
public:
    iModelObjectVis(iModelObject* parent);

    virtual void OnCreate();
    virtual void OnDestroy();

    virtual void OnRenderFrame();

    virtual iModelObject *GetParent() const { return m_Parent; }

    virtual cSprite3D *GetSprite3D() const { return m_Sprite; }
    // Position in parent space
    virtual vec3 GetPosOffset() const { return m_Offset; };
    virtual void SetPosOffset(const vec3 &vec) { m_Offset = vec; };

protected:

    virtual bool SetTexture(const cTexture &texture);
    
    virtual bool InitializeSprite();
    virtual void DestroySprite();

private:

    vec3 m_Offset;

    iModelObject *m_Parent;    

    cSprite3D *m_Sprite;
};