#pragma once

#include <string>
#include "RenderableBase.h"
#include "Property.h"
#include "vec.h"
#include "Texture.h"

class vSprite3DProperties;

class cSprite3D : public vRenderable
{
#ifndef _WIN32
    typedef vRenderable __super;
#endif

public:
    cSprite3D();

    virtual void PrepareForRendering();

    vSprite3DProperties *GetProperties() const;
    
    void SetTexture(const std::string &tex) { m_Texture = tex; }
    std::string GetTexture() const { return m_Texture; }

    PROPERTY(vec3, Position, vec3::ZERO);

protected:

    virtual bool CreateRenderingProperties();

private:
    std::string m_Texture;
};

class vSprite3DProperties : public vRenderingPropertiesBase
{
public:
    vSprite3DProperties();
    virtual ~vSprite3DProperties();

    virtual void Synchronize(const vRenderable *object);

    cTexture GetTexture() const { return m_Texture; }
    void SetTexture(const cTexture &tex) { m_Texture = tex; }
    void DefaultTexture() { m_Texture = cTexture(NULL); }

    PROPERTY(vec3, Position, vec3::ZERO);    
    
private:

    std::string m_TextureName;
    cTexture    m_Texture;
};