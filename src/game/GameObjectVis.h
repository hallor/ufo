#pragma once
#include "vec.h"

class iGameObject;
struct SDL_Surface;

class iGameObjectVis
{
public:
    iGameObjectVis(iGameObject *parent);
    virtual ~iGameObjectVis();

    virtual void OnCreate() {};
    virtual void OnDestroy() {};

    virtual iGameObject *GetParent() const { return m_Parent; }

    SDL_Surface *GetTexture() const { return m_Texture; }
    void SetTexture(SDL_Surface *surf) { m_Texture = surf; }
    
    // Position in parent space
    virtual vec3 GetPosOffset() const { return m_Offset; };
    virtual void SetPosOffset(const vec3 &vec) { m_Offset = vec; };

    virtual void Update(float dt) const {};

private:

    vec3 m_Offset;

    iGameObject *m_Parent;    

    // TODO: change to cTexture, make loading through texture manager
    SDL_Surface *m_Texture;
};