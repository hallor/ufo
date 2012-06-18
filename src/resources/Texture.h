#pragma once
#include "compat_sdl.h"
#include "ResourceBase.h"
#include "vec.h"

class vTextureResource : public vResource<SDL_Surface*>
{
public:
    vTextureResource(SDL_Surface* const &res);
    ~vTextureResource();

    virtual bool IsValid() const;

    vec3 GetSize() const;

protected:
};

class cTexture
{
public:
    cTexture(vTextureResource *res);
    cTexture(const cTexture &other);
    cTexture& operator = (const cTexture &other);

    SDL_Surface *Get() const;
    vTextureResource *GetRawResource() const { return m_Resource; }

    vec3 GetSize() const { return GetRawResource() ? GetRawResource()->GetSize() : vec3::ZERO; }

    void Release() { delete this; }

protected:

    vTextureResource *m_Resource;
};
