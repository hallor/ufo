#pragma once
#include "compat_sdl.h"
#include <vector>
#include "RendererBase.h"
#include "vec.h"
#include "Texture.h"

class vSprite3DProperties;
class iCamera;
class iGameObject;

struct SRenderData
{
public:
    SRenderData(const vec3& pos, const cTexture& tex)
    : m_Pos(pos)
    , m_Texture(tex)
    {

    }

    vec3 m_Pos; // World pos
    cTexture m_Texture; // Texture to use
};

class vGameObjectRenderer : public iRendererBase
{
public:
    vGameObjectRenderer();
    virtual ~vGameObjectRenderer();

    virtual bool Initialize(const cTexture &error_texture);

    virtual void SetCamera(iCamera *cam) { m_Camera = cam; }

    virtual bool IsValid() const;

    virtual void OnFrame(float dt);

    virtual void Render(vRenderable *object);
    virtual void Render(iGameObject *igo);
    virtual void Render(const SRenderData& data) { m_RenderData.push_back(data); }

    virtual void SetOffset(const vec3 &vec) { m_Offset = vec; }

    iCamera *GetCamera() const { return m_Camera; }

protected:

    bool InitSDL();

    void SortRenderingData();

    vec3 m_Offset;

    iCamera *m_Camera;
    SDL_Surface *m_MainSurface;

    cTexture m_ErrorTexture;

    std::vector<SRenderData> m_RenderData;   
};
