#pragma once
#include <string>
#include "vec.h"

class iGameObject;
class cSprite3D;

class iGameObjectVis
{
public:
    iGameObjectVis(iGameObject *parent);
    virtual ~iGameObjectVis();

    virtual void OnCreate() {};
    virtual void OnDestroy();

    virtual iGameObject *GetParent() const { return m_Parent; }

    virtual cSprite3D *GetSprite3D() const { return m_Sprite; }
    // Position in parent space
    virtual vec3 GetPosOffset() const { return m_Offset; };
    virtual void SetPosOffset(const vec3 &vec) { m_Offset = vec; };

    virtual void Update(float dt) const {};

protected:

    virtual bool SetTexture(const std::string &texture);
    
    virtual bool InitializeSprite();
    virtual void DestroySprite();

private:

    vec3 m_Offset;

    iGameObject *m_Parent;    

    cSprite3D *m_Sprite;
};